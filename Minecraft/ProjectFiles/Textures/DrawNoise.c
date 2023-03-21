#include <stdio.h>
#include <math.h>

#include "../Textures/Headers/TextureHeaders.h"
#include "../Utils/Headers/UtilsHeaders.h"
#include "../Generators/Noises/Headers/NoisesHeaders.h"
#include "../Generators/Chunk/Headers/ChunkHeaders.h"

#define MaxNoiseValue 0.864365
#define MinNoiseValue -0.864366

float InvLerp(float a, float b, float v);
void InitNoise(SimplexNoiseObj* noise);

//struct osn_context* ctx;

unsigned char* pixels;

void InitNoise(SimplexNoiseObj* noise)
{
    open_simplex_noise(200, noise->ctx);
    
    float max = 0;
    float min = 0;

    float amplitude = noise->amplitudeVal;
    float frequency = noise->frequencyVal;

    for (int i = 0; i < noise->octaves; i++)
    {
        max += (float) (MaxNoiseValue * 2 - 1) * amplitude;
        min += (float) (MinNoiseValue * 2 - 1) * amplitude;

        amplitude *= noise->persistance;
        frequency *= noise->lacunarity;
    }

    noise->maxNoiseHeight = max;
    noise->minNoiseHeight = min;
}


BlockTypeEnum GetBlockType(int height, SimplexNoiseObj* noise)
{
    ColorScheme* colorScheme = noise->colorScheme;

    if (!colorScheme->useBlock)
        return Air;

    Scheme* scheme = colorScheme->begin;
    while (height > scheme->limit)
        scheme = scheme->next;

    return scheme->block;
}

float GetSingleNoiseVal(int x, int y, BlockInfoStruct* block, SimplexNoiseObj* noise)
{

    float amplitude = noise->amplitudeVal;
    float frequency = noise->frequencyVal;
    float noiseHeight = 0;

    for (int i = 0; i < noise->octaves; i++)
    {
        float sampleX = (x + 20) / noise->scale * frequency;
        float sampleY = (y - 40) / noise->scale * frequency;

        float noiseValue = (float)open_simplex_noise2(*noise->ctx, sampleX, sampleY) * 2 - 1.0f;

        noiseHeight += noiseValue * amplitude;
        amplitude *= noise->persistance;
        frequency *= noise->lacunarity;
    }

    float height = InvLerp(noise->minNoiseHeight, noise->maxNoiseHeight, noiseHeight);

    if (block == NULL)
        return height;

    block->height = (int)height;
    block->blockType = GetBlockType((int)height, noise);

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
            float frequency = noise->frequencyVal;
            float noiseHeight = 0;

            for (int i = 0; i < noise->octaves; i++)
            {
                float sampleX = (col-halfWidth + 20) / noise->scale * frequency;
                float sampleY = (raw-halfHeight -40) / noise->scale * frequency;

                float noiseValue = (float)open_simplex_noise2(*noise->ctx, sampleX, sampleY) * 2 - 1;

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

            BlockInfoStruct* block = blocks[(raw - y) * ChunkSize + (col - x)];

            continue;
            block->height = (int)height;
            block->blockType = GetBlockType((int)height, noise);
        }
    }

    free(map);
}

int chunksSize = ChunkSize * ChunkView * 2;


void DrawNoise(SimplexNoiseObj* noise)
{    
    for (int y = 0; y < ChunkView; y+=16) {
        for (int x = 0; x < ChunkView; x+= 16) {
            GetNoiseMap(x-ChunkSize*ChunkView, y-ChunkSize*ChunkView, noise, noise->blocks[y*ChunkView + x]);            
        }
    }
    CreateBMP(noise);
}

float InvLerp(float a, float b, float v)
{
    return (v - a) / (b - a);
}