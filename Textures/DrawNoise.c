#include <stdio.h>
#include <math.h>
struct SimplexNoiseObj;
#include "../Textures/Headers/TextureHeaders.h"
#include "../Utils/Headers/UtilsHeaders.h"
#include "../Generators/Noises/Headers/NoisesHeaders.h"
#include "../Generators/Chunk/Headers/ChunkHeaders.h"

#define MaxNoiseValue 0.864366
#define MinNoiseValue -0.864366

float InvLerp(float a, float b, float v);
void InitNoise(SimplexNoiseObj* noise);
float scaledOpenSimplexNoise2D(SimplexNoiseObj* noise, float x, float y);

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

    //noise->maxNoiseHeight = max;
    //noise->minNoiseHeight = min;
}


BlockTypeEnum GetBlockType(float height, SimplexNoiseObj* noise, BlockInfoStruct* block)
{
    ColorScheme* colorScheme = noise->colorScheme;

    if (!colorScheme->useBlock)
    {
        return Air;
    }

    Scheme* scheme = colorScheme->begin;
    while (height < scheme->limit)
        scheme = scheme->next;

    block->blockType = scheme->block;
    block->pattern = colorScheme->patterns[scheme->block];

    return Air;
}

float GetSingleNoiseVal(int x, int y, BlockInfoStruct* block, SimplexNoiseObj* noise)
{
    float amplitude = noise->amplitudeVal;
    float frequency = noise->scale;
    float amplitudeSum = 0;
    float noiseHeight = 0;
    float noiseSum = 0;

    for (int i = 0; i < noise->octaves; i++)
    {
        //noiseSum += scaledOpenSimplexNoise2D(noise, x, y, frequency) * amplitude;
        noiseSum += scaledOpenSimplexNoise2D(noise, x, y) * amplitude;
        amplitudeSum += amplitude;
        amplitude *= noise->persistance;
        frequency *= noise->lacunarity;
    }

    noiseHeight = noiseSum / amplitudeSum;

    float height = InvLerp(noise->minNoiseHeight, noise->maxNoiseHeight, noiseHeight);

    if (block == NULL)
        return height;

    block->height = height;

    //Debug
    block->blockType = Grass;
    return height;

    GetBlockType(height, noise, block);
    return height;
}

void GetNoiseMap(int x, int y, SimplexNoiseObj* noise, BlockInfoStruct** blocks)
{
    float* map = malloc(sizeof(float)*ChunkSize*ChunkSize);

    for (int raw = y; raw < ChunkSize + y; raw++) {
        for (int col = x; col < ChunkSize + x; col++) {

            float halfHeight = 0;
            float halfWidth = 0;

            float amplitude = noise->amplitudeVal;
            float frequency = noise->scale;
            float noiseHeight = 0;

            for (int i = 0; i < noise->octaves; i++)
            {
                float sampleX = (col-halfWidth + 20) * frequency;
                float sampleY = (raw-halfHeight -40) * frequency;

                float noiseValue = scaledOpenSimplexNoise2D(noise, sampleX, sampleY);

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

            block->height = height;


            //Debug
            blocks[(raw - y) * ChunkSize + (col - x)] = block;
            continue;

            GetBlockType(height, noise, block);

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

float scaledOpenSimplexNoise2D(SimplexNoiseObj* noise, float x, float y) {
    return scaleRange((float)open_simplex_noise2(noise, x, y), MinNoiseValue, MaxNoiseValue, -1.0, 1.0);
}
