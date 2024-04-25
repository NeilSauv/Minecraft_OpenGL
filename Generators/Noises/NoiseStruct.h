#ifndef NOISE_STRUCT
#define NOISE_STRUCT

typedef enum NoiseNameEnum
{
    TERRAIN,
    BIOME_TEMP,
    BIOME_HUM
} NoiseNameEnum;

typedef struct SimplexNoiseObj
{
    enum NoiseNameEnum name;
    struct osn_context *ctx;
    struct ColorScheme *colorScheme;
    struct BlockInfoStruct ***blocks;
    struct RGBH ***noiseMap;
    float scale;
    int octaves;
    float persistance;
    float lacunarity;
    float amplitude;
    float maxNoiseHeight;
    float minNoiseHeight;
} SimplexNoiseObj;

extern struct SimplexNoiseObj *heightNoise;
extern struct SimplexNoiseObj *temperatureNoise;
extern struct SimplexNoiseObj *rainingNoise;

extern int blockPatterns[42];

void InitNoiseStruct();

void CompleteNoiseMap(struct SimplexNoiseObj *noise);

void InitBlockPattern(struct SimplexNoiseObj *noise);

struct RGBH ***InitNoiseMap();

#endif
