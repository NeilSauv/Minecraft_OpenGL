#ifndef BIOME_GENERATOR
#define BIOME_GENERATOR

#include <Generators/Noises/NoiseStruct.h>
#include <Textures/Block.h>

extern unsigned char *biomeAtlas;

void InitBiomeAtlas(void);

void BiomeSetup(struct SimplexNoiseObj *heightNoise,
                struct SimplexNoiseObj *temperatureNoise,
                struct SimplexNoiseObj *rainingNoise);

enum BiomeType
{
    BIOME_ICE,
    BIOME_SNOWY_TUNDRA,
    BIOME_SNOWY_TAIGA,
    BIOME_GIANT_TREE_TAIGA,
    BIOME_TAIGA,
    BIOME_FOREST,
    BIOME_PLAINS,
    BIOME_SUNFLOWER_PLAINS,
    BIOME_FLOWER_FOREST,
    BIOME_BIRCH_FOREST,
    BIOME_DARK_FOREST,
    BIOME_SAKURA,
    BIOME_JUNGLE,
    BIOME_SAVANA,
    BIOME_DESERT,
    _BIOME_COUNT
};

typedef struct BiomeScheme
{
    enum BiomeType type;
    struct RGB *color;
    enum BlockTypeEnum block;
} BiomeScheme;

#endif
