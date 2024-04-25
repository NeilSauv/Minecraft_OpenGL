#include "DrawNoise.h"

#include <Textures/Block.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Generators/Noises/SimplexNoise.h"
#include "Textures/BlockDef.h"
#include "Textures/ColorMap.h"
struct SimplexNoiseObj;

#include <Generators/Noises/NoiseStruct.h>

#define MaxNoiseValue 0.864366
#define MinNoiseValue -0.864366

float InvLerp(float a, float b, float v);
void InitNoise(SimplexNoiseObj *noise);
float scaledOpenSimplexNoise2D(SimplexNoiseObj *noise, float x, float y,
                               float scale);
float scaleRange(float value, float fromLow, float fromHigh, float toLow,
                 float toHigh);
unsigned char *pixels;

BlockTypeEnum GetBlockType(float height, SimplexNoiseObj *noise,
                           struct BlockInfoStruct *block)
{
    struct ColorScheme *colorScheme = noise->colorScheme;

    // Return 'Air' if blocks are not used in the color scheme
    if (!colorScheme->useBlock)
        return BLOCK_AIR;

    Scheme *scheme = colorScheme->begin;
    Scheme *lastValidScheme = NULL;

    while (scheme)
    {
        if (height > scheme->limit)
        {
            lastValidScheme = scheme; // Keep track of the last valid scheme
            break;
        }
        scheme = scheme->next;
    }

    // If no valid scheme is found, use the last scheme in the list if available
    if (!lastValidScheme)
    {
        if (scheme == NULL)
        { // Check if we've reached the end of the list without finding a match
            scheme = colorScheme->end; // Use the last scheme as a fallback
        }
        lastValidScheme = scheme;
    }

    // Apply the block type and pattern based on the found scheme
    if (lastValidScheme)
    {
        block->blockType = lastValidScheme->block;
        block->pattern = colorScheme->patterns[lastValidScheme->block];
        return block->blockType;
    }
    else
    {
        // Fallback to default block type and pattern if somehow no scheme was
        // valid
        block->blockType = BLOCK_GRASS_BLOCK;
        block->pattern = NULL;
        return BLOCK_GRASS_BLOCK;
    }
}

float GetSingleNoiseVal(int x, int y, BlockInfoStruct *block,
                        SimplexNoiseObj *noise)
{
    float amplitude = noise->amplitude;
    float scale = noise->scale;
    float noiseHeight = 0;

    for (int i = 0; i < noise->octaves; i++)
    {
        float sampleX = x * scale;
        float sampleY = y * scale;

        float noiseValue =
            scaledOpenSimplexNoise2D(noise, sampleX, sampleY, scale);

        noiseHeight += noiseValue * amplitude;
        amplitude *= noise->persistance;
        scale *= noise->lacunarity;
    }

    float height = scaleRange(noiseHeight, -1, 1, noise->minNoiseHeight,
                              noise->maxNoiseHeight);
    height = fmaxf(fminf(height, noise->maxNoiseHeight), noise->minNoiseHeight);

    if (block)
    {
        block->height = height;
        block->blockType = GetBlockType(height, noise, block);
    }
    return height;
}

void GetNoiseMap(int x, int y, SimplexNoiseObj *noise, BlockInfoStruct **blocks)
{
    float *map = malloc(sizeof(float) * ChunkSize * ChunkSize);
    if (!map)
    {
        printf("map malloc failed");
        return;
    }

    for (int raw = y; raw < ChunkSize + y; raw++)
    {
        for (int col = x; col < ChunkSize + x; col++)
        {
            float amplitude = noise->amplitude;
            float scale = noise->scale;
            float noiseHeight = 0;

            for (int i = 0; i < noise->octaves; i++)
            {
                float sampleX = col * scale;
                float sampleY = raw * scale;

                float noiseValue =
                    scaledOpenSimplexNoise2D(noise, sampleX, sampleY, scale);

                noiseHeight += noiseValue * amplitude;
                amplitude *= noise->persistance;
                scale *= noise->lacunarity;
            }

            map[(raw - y) * ChunkSize + (col - x)] = noiseHeight;
        }
    }

    for (int raw = y; raw < ChunkSize + y; raw++)
    {
        for (int col = x; col < ChunkSize + x; col++)
        {
            float height = map[(raw - y) * ChunkSize + (col - x)];
            height = scaleRange(height, -1, 1, noise->minNoiseHeight,
                                noise->maxNoiseHeight);
            height = fmaxf(fminf(height, noise->maxNoiseHeight),
                           noise->minNoiseHeight);

            BlockInfoStruct *block = malloc(sizeof(BlockInfoStruct));
            if (!block)
                continue;

            block->height = height;
            block->blockType = GetBlockType(height, noise, block);

            blocks[(raw - y) * ChunkSize + (col - x)] = block;
        }
    }

    free(map);
}

int chunksSize = ChunkSize * ChunkView * 2;

void DrawNoise(SimplexNoiseObj *noise, char *name)
{
    CreateBMP(noise, name);
}

float InvLerp(float a, float b, float v)
{
    return (v - a) / (b - a);
}

float scaleRange(float value, float fromLow, float fromHigh, float toLow,
                 float toHigh)
{
    float scaledValue = (value - fromLow) / (fromHigh - fromLow);
    return toLow + scaledValue * (toHigh - toLow);
}

float scaledOpenSimplexNoise2D(SimplexNoiseObj *noise, float x, float y,
                               float scale)
{
    x *= scale;
    y *= scale;
    float noiseVal = (float)open_simplex_noise2(noise, x, y);
    return scaleRange(noiseVal, -1.0f, 1.0f, noise->minNoiseHeight,
                      noise->maxNoiseHeight);
}
