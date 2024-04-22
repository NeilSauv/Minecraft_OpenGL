#ifndef BIOME_GENERATOR
#define BIOME_GENERATOR

#include <Generators/Noises/NoiseStruct.h>

extern unsigned char *biomeAtlas;

void InitBiomeAtlas(void);

void BiomeSetup(struct SimplexNoiseObj *heightNoise,
                struct SimplexNoiseObj *temperatureNoise,
                struct SimplexNoiseObj *rainingNoise);

enum BiomeType
{
    ICE,
    SNOWY_TUNDRA,
    SNOWY_TAIGA,
    GIANT_TREE_TAIGA,
    TAIGA,
    FOREST,
    PLAINS,
    SUNFLOWER_PLAINS,
    FLOWER_FOREST,
    BIRCH_FOREST,
    DARK_FOREST,
    SAKURA,
    JUNGLE,
    SAVANA,
    DESERT,
    _BIOME_COUNT
};

typedef struct BiomeScheme
{
    enum BiomeType type;
    struct RGB *color;
    enum BlockTypeEnum block;
} BiomeScheme;

#endif
