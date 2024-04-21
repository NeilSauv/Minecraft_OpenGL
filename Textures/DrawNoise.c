#include <stdio.h>
#include <math.h>
#include "Headers/Block.h"
struct SimplexNoiseObj;
#include "../Textures/Headers/TextureHeaders.h"
#include "../Utils/Headers/UtilsHeaders.h"
#include "../Generators/Noises/Headers/NoisesHeaders.h"
#include "../Generators/Chunk/Headers/ChunkHeaders.h"
#define MaxNoiseValue 0.864366
#define MinNoiseValue -0.864366

float InvLerp(float a, float b, float v);
void InitNoise(SimplexNoiseObj* noise);
float scaledOpenSimplexNoise2D(SimplexNoiseObj* noise, float x, float y, float frequency);

unsigned char* pixels;

void InitNoise(SimplexNoiseObj* noise)
{
    float max = 0;
    float min = 0;

    float amplitude = noise->amplitudeVal;
    float frequency = noise->scale;

    for (int i = 0; i < noise->octaves; i++)
    {
        max += (float) (MaxNoiseValue) * amplitude;
        min += (float) (MinNoiseValue) * amplitude;

        amplitude *= noise->persistance;
        frequency *= noise->lacunarity;
    }

    noise->scale = frequency;
    noise->maxNoiseHeight = max;
    noise->minNoiseHeight = min;
}


BlockTypeEnum GetBlockType(float height, SimplexNoiseObj* noise, BlockInfoStruct* block)
{
    ColorScheme* colorScheme = noise->colorScheme;

    // Return 'Air' if blocks are not used in the color scheme
    if (!colorScheme->useBlock)
        return Air;

    Scheme* scheme = colorScheme->begin;
    Scheme* lastValidScheme = NULL;

    while (scheme) {
        if (height > scheme->limit) {
            lastValidScheme = scheme; // Keep track of the last valid scheme
            break;
        }
        scheme = scheme->next;
    }

    // If no valid scheme is found, use the last scheme in the list if available
    if (!lastValidScheme) {
        if (scheme == NULL) { // Check if we've reached the end of the list without finding a match
            scheme = colorScheme->end; // Use the last scheme as a fallback
        }
        lastValidScheme = scheme;
    }

    // Apply the block type and pattern based on the found scheme
    if (lastValidScheme) {
        block->blockType = lastValidScheme->block;
        block->pattern = colorScheme->patterns[lastValidScheme->block];
        return block->blockType;
    } else {
        // Fallback to default block type and pattern if somehow no scheme was valid
        block->blockType = Grass;
        block->pattern = NULL;
        return Grass;
    }
}float GetSingleNoiseVal(int x, int y, BlockInfoStruct* block, SimplexNoiseObj* noise)
{
    float amplitude = noise->amplitudeVal;
    float frequency = noise->scale;
    float amplitudeSum = 0;
    float noiseSum = 0;

    for (int i = 0; i < noise->octaves; i++)
    {
        noiseSum += scaledOpenSimplexNoise2D(noise, x, y, frequency) * amplitude;
        amplitudeSum += amplitude;
        amplitude *= noise->persistance;
        frequency *= noise->lacunarity;
    }
    float noiseHeight = noiseSum / amplitudeSum;

    float height = InvLerp(noise->minNoiseHeight, noise->maxNoiseHeight, noiseHeight);

    if (block)
    {
        block->height = height;
        block->blockType = GetBlockType(height, noise, block);
    }

    return height;
}

void GetNoiseMap(int x, int y, SimplexNoiseObj* noise, BlockInfoStruct** blocks)
{
    float* map = malloc(sizeof(float) * ChunkSize * ChunkSize);

    for (int raw = y; raw < ChunkSize + y; raw++) {
        for (int col = x; col < ChunkSize + x; col++) {

            float amplitude = noise->amplitudeVal;
            float frequency = noise->scale;
            float noiseHeight = 0;

            for (int i = 0; i < noise->octaves; i++)
            {
                float sampleX = (col + 20) * frequency;
                float sampleY = (raw -40) * frequency;

                float noiseValue = scaledOpenSimplexNoise2D(noise, sampleX, sampleY, frequency);

                noiseHeight += noiseValue * amplitude;
                amplitude *= noise->persistance;
                frequency *= noise->lacunarity;
            }

            map[(raw - y) * ChunkSize + (col - x)] = noiseHeight;
        }
    }

    for (int raw = y; raw < ChunkSize + y; raw++) {
        for (int col = x; col < ChunkSize + x; col++) {
            float height = InvLerp(noise->minNoiseHeight, noise->maxNoiseHeight, map[(raw - y) * ChunkSize + (col - x)]);

            BlockInfoStruct* block = malloc(sizeof(BlockInfoStruct));
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

void DrawNoise(SimplexNoiseObj* noise, char* name)
{
    CreateBMP(noise, name);
}

float InvLerp(float a, float b, float v)
{
    return (v - a) / (b - a);
}

float scaleRange(float value, float fromLow, float fromHigh, float toLow, float toHigh) {
    float scaledValue = (value - fromLow) / (fromHigh - fromLow);
    return toLow + scaledValue * (toHigh - toLow);
}

float scaledOpenSimplexNoise2D(SimplexNoiseObj* noise, float x, float y, float frequency) {
    x *= frequency;
    y *= frequency;
    return scaleRange((float)open_simplex_noise2(noise, x, y), MinNoiseValue, MaxNoiseValue, -1.0, 1.0);
}
