#include "BiomeGenerator.h"

#include <Textures/BitmapCreator.h>
#include <Textures/TextureSet.h>
#include <stdlib.h>

#include "Textures/Block.h"
#include "Textures/ColorMap.h"

unsigned char *biomeAtlas = 0;

void InitBiomeAtlas(void)
{
    biomeAtlas = ReadBPM("Textures/Pictures/biomeAtlas.bmp");
}

static void InitBiomeScheme(struct BiomeScheme **biome_scheme,
                            enum BiomeType type, enum BlockTypeEnum block,
                            int red, int green, int blue)
{
    struct BiomeScheme *scheme = malloc(sizeof(struct BiomeScheme));
    if (!scheme)
        return;
    scheme->block = block;
    scheme->type = type;
    scheme->color = malloc(sizeof(struct RGB));
    if (!scheme->color)
        return;
    scheme->color->blue = blue;
    scheme->color->red = red;
    scheme->color->green = green;
    biome_scheme[type] = scheme;
}

void BiomeSetup(struct SimplexNoiseObj *heightNoise,
                struct SimplexNoiseObj *temperatureNoise,
                struct SimplexNoiseObj *rainingNoise)
{
    struct BiomeScheme **biome_scheme =
        malloc(sizeof(struct BiomeScheme *) * _BIOME_COUNT);
    if (!biome_scheme)
        return;

    InitBiomeScheme(biome_scheme, BIOME_ICE, BLOCK_AIR, 180, 220, 220);
    InitBiomeScheme(biome_scheme, BIOME_SNOWY_TUNDRA, BLOCK_AIR, 255, 255, 255);
    InitBiomeScheme(biome_scheme, BIOME_SNOWY_TAIGA, BLOCK_AIR, 49, 85, 74);
    InitBiomeScheme(biome_scheme, BIOME_GIANT_TREE_TAIGA, BLOCK_AIR, 89, 102,
                    81);
    InitBiomeScheme(biome_scheme, BIOME_TAIGA, BLOCK_AIR, 11, 102, 89);
    InitBiomeScheme(biome_scheme, BIOME_FOREST, BLOCK_AIR, 5, 102, 33);
    InitBiomeScheme(biome_scheme, BIOME_PLAINS, BLOCK_AIR, 141, 179, 96);
    InitBiomeScheme(biome_scheme, BIOME_SAKURA, BLOCK_AIR, 255, 0, 236);
    InitBiomeScheme(biome_scheme, BIOME_SUNFLOWER_PLAINS, BLOCK_AIR, 118, 219,
                    136);
    InitBiomeScheme(biome_scheme, BIOME_FLOWER_FOREST, BLOCK_AIR, 45, 172, 73);
    InitBiomeScheme(biome_scheme, BIOME_BIRCH_FOREST, BLOCK_AIR, 48, 116, 68);
    InitBiomeScheme(biome_scheme, BIOME_DARK_FOREST, BLOCK_AIR, 104, 121, 66);
    InitBiomeScheme(biome_scheme, BIOME_JUNGLE, BLOCK_AIR, 82, 123, 9);
    InitBiomeScheme(biome_scheme, BIOME_SAVANA, BLOCK_AIR, 189, 178, 95);
    InitBiomeScheme(biome_scheme, BIOME_DESERT, BLOCK_AIR, 250, 148, 24);

    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            int temperature =
                (float)(temperatureNoise->noiseMap[y][x]->height + 1) / 2 * 250;
            int raining =
                (float)(rainingNoise->noiseMap[y][x]->height + 1) / 2 * 250;

            heightNoise->noiseMap[y][x]->blue =
                biomeAtlas[temperature * 3 * 250 + raining * 3 + 2];
            heightNoise->noiseMap[y][x]->green =
                biomeAtlas[temperature * 3 * 250 + raining * 3 + 1];
            heightNoise->noiseMap[y][x]->red =
                biomeAtlas[temperature * 3 * 250 + raining * 3 + 3];
        }
    }
}
