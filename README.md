# Minecraft OpenGL

A Minecraft-inspired 3D voxel world engine written in C using OpenGL. Implements procedural terrain generation with Simplex noise, a biome system based on temperature/humidity maps, chunk-based rendering with instancing, and basic player interaction (movement, gravity, block destruction).

---

## Table of Contents

- [Overview](#overview)
- [Architecture](#architecture)
- [Directory Structure](#directory-structure)
- [Dependencies](#dependencies)
- [Building](#building)
- [Core Systems](#core-systems)
  - [Noise Generation](#noise-generation)
  - [Biome System](#biome-system)
  - [Chunk System](#chunk-system)
  - [Rendering Pipeline](#rendering-pipeline)
  - [Player System](#player-system)
  - [Physics & Collision](#physics--collision)
  - [Block Destruction](#block-destruction)
- [Data Structures](#data-structures)
- [Configuration Constants](#configuration-constants)
- [Shader Overview](#shader-overview)
- [Texture Atlas Generator](#texture-atlas-generator)
- [Debug Utilities](#debug-utilities)

---

## Overview

The engine generates an infinite-looking 3D voxel world procedurally at startup using layered Simplex noise. The world is divided into 16×16×256 chunks; a 30×30 grid of chunks is kept active around the player and updated as the camera moves. The biome of each column is determined by two independent noise maps (temperature and humidity) sampled against a 250×250 biome atlas image.

**Key capabilities:**

- Procedural terrain via octave Simplex noise (height, temperature, humidity)
- 15 biome types assigned from a 2D biome atlas texture
- 650+ block types with per-face texture atlas support
- Real-time instanced chunk rendering with face culling state
- First-person camera with yaw/pitch mouse look
- Physics: gravity, jumping, ground collision
- Block raycasting and destruction with persistent per-region state
- BMP file output for noise map visualization/debugging

---

## Architecture

```text
main.c
  │
  ├── Noise Init (NoiseStruct, SimplexNoise)
  │     three SimplexNoiseObj: heightNoise, temperatureNoise, rainingNoise
  │
  ├── Color/Block Scheme Init (ColorMap)
  │     height → block type mapping (water / sand / grass / stone / snow)
  │
  ├── Noise Map Computation (DrawNoise)
  │     per-pixel octave Simplex noise → RGBH array
  │
  ├── Biome Setup (BiomeGenerator)
  │     temperature × humidity → biome atlas lookup → colorize heightNoise
  │
  ├── Window & OpenGL Init (GLFW/glad)
  │
  ├── Chunk Generation (ChunkGenerator, ChunkManager)
  │     900 chunks (30×30) around origin, GPU buffers allocated
  │
  ├── Texture Load (TextureSet)
  │     atlas.png → GL_TEXTURE0
  │
  └── Render Loop
        ProcessMoves / ProcessMouse
        Update()        ← dynamic chunk sliding window
        DrawChunk()     ← instanced draw
```

---

## Directory Structure

```text
Minecraft_OpenGL/
├── main.c                          # Entry point, render loop
├── CMakeLists.txt                  # Build system (CMake 3.16+)
│
├── Generators/
│   ├── Noises/
│   │   ├── NoiseStruct.h/.c        # SimplexNoiseObj definition, global instances
│   │   └── SimplexNoise.h/.c       # OpenSimplex 2D/3D/4D implementation
│   └── Chunk/
│       ├── BiomeGenerator.h/.c     # Biome assignment and atlas lookup
│       ├── ChunkGenerator.h/.c     # Mesh generation, GPU buffer upload
│       ├── ChunkManager.h/.c       # Sliding-window chunk lifecycle
│       └── Region.h/.c             # Per-region block modification tracking
│
├── Player/
│   ├── Camera.h/.c                 # Yaw/pitch, forward/right/up vectors
│   ├── Controller.h/.c             # WASD movement, sprint, god mode, gravity
│   └── Destroy.h/.c                # Block destruction on mouse click
│
├── Physics/
│   ├── Collisions.h/.c             # Height-based ground collision
│   └── Ray.h/.c                    # Raycasting against terrain
│
├── Textures/
│   ├── Block.h/.c                  # BlockTypeEnum (650+ types), InitBlockPattern()  [generated]
│   ├── BlockDef.h/.c               # BlockPattern (6-face atlas indices)
│   ├── ColorMap.h/.c               # Scheme linked list: height threshold → block/color
│   ├── DrawNoise.h/.c              # Octave noise evaluation, block assignment
│   ├── TextureSet.h/.c             # OpenGL texture creation (atlas.png)
│   └── BitmapCreator.h/.c          # Debug BMP output (height, RGB, biome maps)
│
├── Shaders/
│   ├── Shader.h/.c                 # Compile/link GLSL, Windows API file read
│   ├── BasicVertices.vert          # Vertex shader (GLSL 4.3)
│   └── BasicColor.frag             # Fragment shader (GLSL 4.3)
│
├── Utils/
│   ├── Errors.h/.c                 # Malloc wrapper with error check
│   ├── FileUtils.h/.c              # Debug log file (Windows API)
│   ├── List.h/.c                   # Dynamic int list
│   └── TimeUtils.h/.c              # Delta time (glfwGetTime based)
│
├── Scripts/
│   └── atlas_generator.py          # Builds Textures/Block.h/.c + atlas.png from Assets/
│
├── Assets/
│   └── Block/                      # Individual 16×16 PNG textures per block face
│
└── opengl/                         # Vendored: glad, cglm, stb_image, stb_vorbis
```

> `Textures/Block.h` and `Textures/Block.c` are **generated** by `atlas_generator.py` and are not committed. CMake runs the script automatically if they are missing.

---

## Dependencies

| Library | Purpose | Location |
| ------- | ------- | -------- |
| GLFW 3 | Window creation, input | `C:/glfw3` (Windows) / system (Linux) |
| glad | OpenGL function loader | `opengl/glad/` (vendored) |
| cglm | C math (vec3, mat4, …) | `opengl/cglm/` (vendored) |
| stb_image | PNG loading (atlas) | `opengl/stb/stb_image.h` (vendored) |
| OpenGL 4.3 | Rendering | Driver / `opengl32.lib` |

**Windows only:**

- Visual Studio 2022 toolchain (MSVC)
- CMake 3.16+
- Python 3 (for atlas generation at first build)
- GLFW installed at `C:\glfw3` with `lib-vc2022` prebuilt binaries

---

## Building

### Windows (MSVC)

```bat
cmake -B build
cmake --build build --config Debug
.\build\Debug\Minecraft.exe
```

CMake automatically:

- Runs `atlas_generator.py` if `Textures/Block.h` or `Textures/Block.c` is missing
- Copies `glfw3.dll` next to the executable
- Copies `Textures/`, `Shaders/`, and `Assets/` next to the executable

### Linux / GCC

```bash
cmake -B build
cmake --build build
./build/Minecraft
```

The `format` target runs `clang-format -i` on all project sources:

```bash
cmake --build build --target format
```

---

## Core Systems

### Noise Generation

Three independent `SimplexNoiseObj` instances are created at startup with different seeds and tuning:

| Instance | Seed | Scale | Octaves | Persistance | Lacunarity | Height range |
| -------- | ---- | ----- | ------- | ----------- | ---------- | ------------ |
| `heightNoise` | 200 | 0.03 | 5 | 0.25 | 2.5 | −0.7 … 1.0 |
| `temperatureNoise` | 201 | 0.09 | 6 | 0.2 | 2.3 | −1.0 … 1.0 |
| `rainingNoise` | 202 | 0.04 | 6 | 0.2 | 2.3 | −1.0 … 1.0 |

**Octave composition** (`DrawNoise.c` → `GetSingleNoiseVal`):

```c
noiseHeight = 0
amplitude   = 1.0
scale       = noise->scale

for each octave:
    noiseHeight += open_simplex_noise2D(x * scale, y * scale) * amplitude
    amplitude   *= persistance    // Reduces high-frequency contribution
    scale       *= lacunarity     // Increases sample frequency
```

The result is normalized to the configured `[minNoiseHeight, maxNoiseHeight]` range and stored in the per-pixel `RGBH.height` field.

The noise map size is `(ChunkView * 2 * ChunkSize)²` = **480 × 480** pixels.

---

### Biome System

Biome assignment uses three noise maps together:

1. **`temperatureNoise`** and **`rainingNoise`** are each normalized to `[0, 250]`.
2. A 250×250 pixel **biome atlas image** (`Textures/Pictures/atlas.png`) encodes one RGB color per (temperature, humidity) combination.
3. `BiomeSetup()` reads the atlas at `(temperature, humidity)` and writes the resulting color back into `heightNoise->noiseMap[y][x]`.

**15 biome types** range from `BIOME_ICE` (cold/dry) through `BIOME_FOREST`, `BIOME_PLAINS`, `BIOME_JUNGLE`, to `BIOME_DESERT` (hot/dry).

The biome color replaces the per-pixel RGB used for visualization; actual block types remain driven by the height-based `ColorScheme`.

---

### Chunk System

#### Static layout

| Constant | Value | Meaning |
| -------- | ----- | ------- |
| `ChunkSize` | 16 | Blocks per edge (X and Z) |
| `ChunkHeight` | 256 | Vertical blocks per chunk |
| `ChunkView` | 15 | Active radius in chunks |

The active world is a **30 × 30** grid of chunks (900 total) centered on the player.

#### Chunk generation (`ChunkGenerator.c`)

Each chunk's blocks are populated by calling `GetSingleNoiseVal(worldX, worldZ)` for every column, which returns a height that selects a `BlockTypeEnum`. The mesh is uploaded as an instanced draw call using:

- **VAO / VBO**: unit cube vertices (position + UV + face index)
- **instanceVBO**: per-block world offset (vec3)
- **renderVBO**: per-block render state `vec4` — block type, face visibility mask, destruction flag, orientation

#### Dynamic sliding window (`ChunkManager.c` → `Update()`)

When the camera crosses a chunk boundary the trailing edge is recycled to the leading edge. Only the newly exposed row or column of chunks is regenerated; the rest remain unchanged.

---

### Rendering Pipeline

**Draw call** (`DrawChunk`): one `glDrawElementsInstanced` per chunk, drawing up to 16×16×256 block instances. Invisible faces (buried or destroyed blocks) are flagged in the `renderVBO` and skipped in the vertex shader.

**Vertex shader** (`BasicVertices.vert`, GLSL 4.3):

- Receives: position, UV, face index, block world offset, render state vec4
- Looks up atlas UV from `blockPatterns[blockType * 6 + faceIndex]`
- Applies model/view/projection transforms
- Supports per-block orientation (the `aRender.w` rotation channel)

**Fragment shader** (`BasicColor.frag`, GLSL 4.3):

- Samples the packed texture atlas at the interpolated UV
- Outputs final color; alpha blending enabled for transparent blocks (water, leaves)

**Texture atlas** (`TextureSet.c`):

- Loaded once at startup via `stb_image`
- Bound to `GL_TEXTURE0` for the duration of the session
- `GL_NEAREST` filtering preserves the pixel art look

---

### Player System

#### Camera (`Camera.h/.c`)

Mouse delta drives yaw and pitch (sensitivity 0.15°/px, pitch clamped to ±89.999°):

```c
forward = { cos(yaw)*cos(pitch), sin(pitch), sin(yaw)*cos(pitch) }
right   = normalize(cross(forward, worldUp))
up      = normalize(cross(right, forward))
```

#### Movement (`Controller.h/.c`)

| Key | Action |
| --- | ------ |
| W / S | Move forward / backward |
| A / D | Strafe left / right |
| Space | Jump (when grounded) |
| Ctrl | Move down (god mode) |
| Shift | Sprint (1.7× speed) |
| G | Toggle god mode (no gravity) |

Speed values: walk = 4.16, sprint ≈ 7.13, god mode = 150 units/s. Movement is delta-time scaled for frame-rate independence.

#### Gravity & jumping

When god mode is off, `Falling()` applies `9.8 m/s²` acceleration each frame until the player is grounded. Jumping sets a target apex height; the character rises until apex then falls back under gravity.

---

### Physics & Collision

`MovesCollisions(targetPos)` (`Physics/Collisions.c`):

1. Evaluates `GetSingleNoiseVal` at the target XZ position to get terrain height.
2. Returns `true` (move allowed) only when `targetPos.y - 2.25 >= terrainHeight` and the player is above `WaterLevel = 0`.
3. Water blocks the player from going underwater in survival mode.

The 2.25-unit offset represents the player's eye-to-feet height.

---

### Block Destruction

`Destroy.c` / `Region.c`:

1. On left-click, `RayCast(cameraPos, forward, maxDist=10, step=0.1)` steps along the view ray.
2. At each step the terrain height is sampled; when the ray intersects solid ground the block coordinates are returned.
3. `AddBlockState(blockPos)` records the destruction in a `DestroyList` inside the owning `regionList`.
4. The chunk containing the block is marked dirty and `UpdateDestroyed()` regenerates its GPU buffers, omitting destroyed blocks.

Regions are `RegionSize × RegionSize` (20×20) block areas. Four regions (`RegionCount = 4`) are maintained, creating an 80×80 block area of persistent modifications.

---

## Data Structures

### `SimplexNoiseObj` (NoiseStruct.h)

```c
typedef struct {
    NoiseNameEnum    name;             // TERRAIN | BIOME_TEMP | BIOME_HUM
    osn_context     *ctx;              // OpenSimplex internal context
    ColorScheme     *colorScheme;      // Height → block/color mapping
    BlockInfoStruct ***blocks;         // [mapH][mapW] block info grid
    RGBH           ***noiseMap;        // [mapH][mapW] height + color output
    float            scale;
    int              octaves;
    float            persistance;
    float            lacunarity;
    float            amplitude;
    float            maxNoiseHeight;
    float            minNoiseHeight;
} SimplexNoiseObj;
```

### `RGBH` (NoiseStruct.h)

```c
typedef struct {
    unsigned int red, green, blue;   // Assigned color (biome or height scheme)
    float        height;             // Normalized noise value
} RGBH;
```

### `ColorScheme` / `Scheme` (ColorMap.h)

A singly-linked list of height thresholds sorted from high to low:

```c
typedef struct Scheme {
    float          limit;     // Height threshold (checked highest first)
    RGB           *color;     // Debug color
    BlockTypeEnum  block;     // Block assigned when height >= limit
    Scheme        *next;
} Scheme;

typedef struct {
    int           size;
    int           useBlock;          // 0 = visualization only, 1 = assign blocks
    Scheme       *begin, *end;
    BlockPattern **patterns;         // Atlas patterns for each block in scheme
} ColorScheme;
```

Default height mapping (terrain):

| Condition | Block |
| --------- | ----- |
| height > 0.85 | BLOCK_SNOW_BLOCK |
| height > 0.80 | BLOCK_STONE |
| height > 0.03 | BLOCK_GRASS_BLOCK |
| height > 0.00 | BLOCK_SAND |
| height ≤ 0.00 | BLOCK_WATER_FLOW |

### `BlockPattern` (BlockDef.h)

```c
typedef struct {
    int topFace;
    int bottomFace;
    int sideOne, sideTwo, sideThree, sideFour;
} BlockPattern;
```

Each field is an index into the packed texture atlas. Generated by `atlas_generator.py`.

### `regionList` / `DestroyList` (Region.h)

```c
typedef struct {
    DestroyList *data;      // Linked list of destroyed block render indices
    DestroyList *next;
} DestroyList;

typedef struct {
    int          x, z;
    DestroyList *destroyList[RegionSize * RegionSize];
} regionList;
```

---

## Configuration Constants

| Constant | Value | File | Description |
| -------- | ----- | ---- | ----------- |
| `ChunkSize` | 16 | ChunkGenerator.h | Blocks per chunk edge (XZ) |
| `ChunkHeight` | 256 | ChunkGenerator.h | Vertical blocks per chunk |
| `ChunkView` | 15 | ChunkManager.h | Active chunk radius |
| `RegionSize` | 20 | Region.h | Blocks per region edge |
| `RegionCount` | 4 | Region.h | Number of active regions |
| `WaterLevel` | 0 | Collisions.h | Y level of the water surface |
| `SCR_WIDTH` | 1920 | main.c | Window width |
| `SCR_HEIGHT` | 1080 | main.c | Window height |

---

## Shader Overview

### `BasicVertices.vert`

GLSL 4.3, per-vertex (pos, UV, face) + per-instance (offset, renderState) attributes:

- `blockPatterns[42]` uniform maps `blockType * 6 + faceIndex` → atlas column
- Handles 4 orientations via `aRender.w` for directional blocks
- Culls faces using the visibility mask in `aRender.y`

### `BasicColor.frag`

- Samples `sampler2D atlas` at interpolated UV
- Passes through texture color directly (no lighting yet)

---

## Texture Atlas Generator

`Scripts/atlas_generator.py` must be run before the first build (CMake calls it automatically):

1. Scans `Assets/Block/*.png` (each image is 16×16 pixels).
2. Recognizes `_top`, `_bottom`, `_side` suffixes to assign per-face textures.
3. Packs all images into a single square atlas PNG → `Textures/Pictures/atlas.png`.
4. Emits `Textures/Block.h` (enum of all block types) and `Textures/Block.c` (`InitBlockPattern()` populating a `BlockPattern` array with atlas indices).

The atlas is loaded once at runtime by `stb_image` and bound to `GL_TEXTURE0`.

---

## Debug Utilities

### BMP output (`BitmapCreator.c`)

Called before the window opens, writes three BMP files to `test/`:

| File | Content |
| ---- | ------- |
| `<name>_BW.bmp` | Grayscale height map |
| `<name>_RGB.bmp` | Height map with block colors |
| `Biomes_RGB.bmp` | Biome atlas lookup result |

### Log file (`Utils/FileUtils.c`)

`OpenLogFile()` creates `print.txt` in the working directory. Helper functions:

- `WriteLine(str)` — raw string
- `WriteLinei(int)` / `WriteLinef(float)` — formatted number
- `PrintVec3/Vec4/Mat4/Arr3` — cglm types to text
- All I/O uses the Windows API (`WriteFile`, `FlushFileBuffers`).

### FPS counter (`main.c`)

Prints `%.2f FPS` to stdout every second via `printf`.
