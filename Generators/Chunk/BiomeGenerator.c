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

    InitBiomeScheme(biome_scheme, ICE, Air, 180, 220, 220);
    InitBiomeScheme(biome_scheme, SNOWY_TUNDRA, Air, 255, 255, 255);
    InitBiomeScheme(biome_scheme, SNOWY_TAIGA, Air, 49, 85, 74);
    InitBiomeScheme(biome_scheme, GIANT_TREE_TAIGA, Air, 89, 102, 81);
    InitBiomeScheme(biome_scheme, TAIGA, Air, 11, 102, 89);
    InitBiomeScheme(biome_scheme, FOREST, Air, 5, 102, 33);
    InitBiomeScheme(biome_scheme, PLAINS, Air, 141, 179, 96);
    InitBiomeScheme(biome_scheme, SAKURA, Air, 255, 0, 236);
    InitBiomeScheme(biome_scheme, SUNFLOWER_PLAINS, Air, 118, 219, 136);
    InitBiomeScheme(biome_scheme, FLOWER_FOREST, Air, 45, 172, 73);
    InitBiomeScheme(biome_scheme, BIRCH_FOREST, Air, 48, 116, 68);
    InitBiomeScheme(biome_scheme, DARK_FOREST, Air, 104, 121, 66);
    InitBiomeScheme(biome_scheme, JUNGLE, Air, 82, 123, 9);
    InitBiomeScheme(biome_scheme, SAVANA, Air, 189, 178, 95);
    InitBiomeScheme(biome_scheme, DESERT, Air, 250, 148, 24);

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
