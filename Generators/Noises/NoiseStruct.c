#include "NoiseStruct.h"

#include <Generators/Chunk/ChunkGenerator.h>
#include <Textures/DrawNoise.h>
#include <stdio.h>
#include <stdlib.h>

#include "Textures/BlockDef.h"
#include "Textures/ColorMap.h"

struct SimplexNoiseObj *heightNoise = NULL;
struct SimplexNoiseObj *temperatureNoise = NULL;
struct SimplexNoiseObj *rainingNoise = NULL;
int blockPatterns[42];

struct RGBH ***InitNoiseMap()
{
    struct RGBH ***noiseMap =
        malloc(sizeof(struct RGBH *) * ChunkView * 2 * ChunkSize);
    if (!noiseMap)
        printf("noiseMap is NULL");

    for (int y = 0; y < ChunkView * 2 * ChunkSize; y++)
    {
        noiseMap[y] = malloc(sizeof(RGBH *) * ChunkView * 2 * ChunkSize);

        for (int x = 0; x < ChunkView * 2 * ChunkSize; x++)
        {
            RGBH *rgbh = malloc(sizeof(RGBH));
            rgbh->height = 0;
            rgbh->red = 0;
            rgbh->green = 0;
            rgbh->blue = 0;

            noiseMap[y][x] = rgbh;
        }
    }

    return noiseMap;
}

BlockInfoStruct ***InitBlockInfoStruct()
{
    BlockInfoStruct ***blocks =
        calloc(ChunkView * ChunkView * 2, sizeof(BlockInfoStruct **));
    if (!blocks)
        printf("blocks is NULL");

    for (int i = 0; i < ChunkView * ChunkView * 2; i++)
    {
        blocks[i] = calloc(ChunkSize * ChunkSize, sizeof(BlockInfoStruct *));

        for (int j = 0; j < ChunkSize * ChunkSize; j++)
        {
            struct BlockInfoStruct *block = malloc(sizeof(BlockInfoStruct));
            block->blockType = BLOCK_AIR;
            block->height = 0;

            blocks[i][j] = block;
        }
    }

    return blocks;
}

void CompleteNoiseMap(SimplexNoiseObj *noise)
{
    RGBH ***rgbh = noise->noiseMap;
    BlockInfoStruct *block = malloc(sizeof(BlockInfoStruct));

    for (int y = 0; y < ChunkView * 2 * ChunkSize; y++)
    {
        for (int x = 0; x < ChunkView * 2 * ChunkSize; x++)
        {
            rgbh[y][x]->height = GetSingleNoiseVal(x, y, block, noise);
            RGB *rgb = GetBlockColor(block, noise);

            rgbh[y][x]->red = rgb->red;
            rgbh[y][x]->green = rgb->green;
            rgbh[y][x]->blue = rgb->blue;
        }
    }

    free(block);
}

void InitNoiseStruct()
{
    // Height Noise Init
    heightNoise = malloc(sizeof(SimplexNoiseObj));
    heightNoise->name = TERRAIN;
    heightNoise->ctx = NULL;
    heightNoise->colorScheme = malloc(sizeof(ColorScheme));
    heightNoise->blocks = InitBlockInfoStruct();
    heightNoise->noiseMap = InitNoiseMap();
    heightNoise->scale = 0.03f;
    heightNoise->octaves = 5;
    heightNoise->persistance = 0.2f;
    heightNoise->lacunarity = 2.5f;
    heightNoise->amplitude = 1.0f;
    heightNoise->maxNoiseHeight = 1.0f;
    heightNoise->minNoiseHeight = -0.7f;

    // Temperature Noise Init
    temperatureNoise = malloc(sizeof(SimplexNoiseObj));
    temperatureNoise->name = BIOME_TEMP;
    temperatureNoise->ctx = NULL;
    temperatureNoise->colorScheme = malloc(sizeof(ColorScheme));
    temperatureNoise->blocks = InitBlockInfoStruct();
    temperatureNoise->noiseMap = InitNoiseMap();
    temperatureNoise->scale = 0.0045f * 20;
    temperatureNoise->octaves = 6;
    temperatureNoise->persistance = 0.25f;
    temperatureNoise->lacunarity = 2.3f;
    temperatureNoise->amplitude = 1.0f;
    temperatureNoise->maxNoiseHeight = 1.0f;
    temperatureNoise->minNoiseHeight = -1.0f;

    // Temperature Noise Init
    rainingNoise = malloc(sizeof(SimplexNoiseObj));
    rainingNoise->name = BIOME_HUM;
    rainingNoise->ctx = NULL;
    rainingNoise->colorScheme = malloc(sizeof(ColorScheme));
    rainingNoise->blocks = InitBlockInfoStruct();
    rainingNoise->noiseMap = InitNoiseMap();
    rainingNoise->scale = 0.002f * 20;
    rainingNoise->octaves = 6;
    rainingNoise->persistance = 0.2f;
    rainingNoise->lacunarity = 2.5f;
    rainingNoise->amplitude = 1.0f;
    rainingNoise->maxNoiseHeight = 1.0f;
    rainingNoise->minNoiseHeight = -1.0f;
}
