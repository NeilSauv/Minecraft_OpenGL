#include "NoiseStruct.h"

#include <Textures/DrawNoise.h>
#include <stdio.h>
#include <stdlib.h>

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
            block->blockType = Air;
            block->height = 0;

            blocks[i][j] = block;
        }
    }

    return blocks;
}

void AddToBlockPattern(BlockTypeEnum block, int top, int bottom, int side1,
                       int side2, int side3, int side4, BlockPattern **patterns)
{
    BlockPattern *pattern = malloc(sizeof(BlockPattern));
    pattern->topFace = top;
    pattern->bottomFace = bottom;
    pattern->sideOne = side1;
    pattern->sideTwo = side2;
    pattern->sideThree = side3;
    pattern->sideFour = side4;

    patterns[block] = pattern;

    blockPatterns[block * 6 + 0] = top;
    blockPatterns[block * 6 + 1] = side1;
    blockPatterns[block * 6 + 2] = side2;
    blockPatterns[block * 6 + 3] = side3;
    blockPatterns[block * 6 + 4] = side4;
    blockPatterns[block * 6 + 5] = bottom;
}

void InitBlockPattern(SimplexNoiseObj *noise)
{
    ColorScheme *colorScheme = noise->colorScheme;
    int size = colorScheme->size;

    BlockPattern **patterns = malloc(sizeof(BlockPattern *) * size);
    colorScheme->patterns = patterns;

    AddToBlockPattern(Grass, 0, 2, 1, 1, 1, 1, patterns);
    AddToBlockPattern(Snow, 3, 5, 4, 4, 4, 4, patterns);
    AddToBlockPattern(Dirt, 2, 2, 2, 2, 2, 2, patterns);
    AddToBlockPattern(Stone, 6, 6, 6, 6, 6, 6, patterns);
    AddToBlockPattern(Water, 7, 7, 7, 7, 7, 7, patterns);
    AddToBlockPattern(Sand, 8, 8, 8, 8, 8, 8, patterns);
}

void CompleteNoiseMap(SimplexNoiseObj *noise)
{
    RGBH ***rgbh = noise->noiseMap;
    BlockInfoStruct *block = malloc(sizeof(BlockInfoStruct));

    for (int y = 0; y < ChunkView * 2 * ChunkSize; y++)
    {
        for (int x = 0; x < ChunkView * 2 * ChunkSize; x++)
        {
            float height = GetSingleNoiseVal(x, y, block, noise);
            rgbh[y][x]->height = height;

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
    heightNoise->scale = 0.0002f;
    heightNoise->octaves = 6;
    heightNoise->persistance = 0.15f;
    heightNoise->lacunarity = 2.5f;
    heightNoise->amplitudeVal = 2.0f;
    heightNoise->frequencyVal = 2.0f;
    heightNoise->maxNoiseHeight = 1.0f;
    heightNoise->minNoiseHeight = -1.0f;

    // Temperature Noise Init
    temperatureNoise = malloc(sizeof(SimplexNoiseObj));
    temperatureNoise->name = TERRAIN;
    heightNoise->ctx = NULL;
    temperatureNoise->colorScheme = malloc(sizeof(ColorScheme));
    temperatureNoise->blocks = InitBlockInfoStruct();
    temperatureNoise->noiseMap = InitNoiseMap();
    temperatureNoise->scale = 0.0001f;
    temperatureNoise->octaves = 3;
    temperatureNoise->persistance = 0.3f;
    temperatureNoise->lacunarity = 2.5f;
    temperatureNoise->amplitudeVal = 1.0f;
    temperatureNoise->frequencyVal = 1.0f;
    temperatureNoise->maxNoiseHeight = 1.0f;
    temperatureNoise->minNoiseHeight = -1.0f;

    // Temperature Noise Init
    rainingNoise = malloc(sizeof(SimplexNoiseObj));
    rainingNoise->name = TERRAIN;
    heightNoise->ctx = NULL;
    rainingNoise->colorScheme = malloc(sizeof(ColorScheme));
    rainingNoise->blocks = InitBlockInfoStruct();
    rainingNoise->noiseMap = InitNoiseMap();
    rainingNoise->scale = 0.0001f;
    rainingNoise->octaves = 3;
    rainingNoise->persistance = 0.3f;
    rainingNoise->lacunarity = 2.5f;
    rainingNoise->amplitudeVal = 1.0f;
    rainingNoise->frequencyVal = 1.0f;
    rainingNoise->maxNoiseHeight = 1.0f;
    rainingNoise->minNoiseHeight = -1.0f;
}
