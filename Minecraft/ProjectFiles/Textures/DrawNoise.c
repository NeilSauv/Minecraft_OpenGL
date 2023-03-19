#include <stdio.h>
#include <math.h>

#include "../Utils/Headers/FileUtils.h"
#include "../Generators/Noises/Headers/SimplexNoise.h"
#include "../Generators/Chunk/Headers/ChunkGenerator.h"
#include "../Generators/Chunk/Headers/ChunkManager.h"
#include "Headers/ColorMap.h"
#include "Headers/BitmapCreator.h"
#include "../Generators/Noises/Headers/NoiseStruct.h"

#define MaxNoiseValue 0.864365
#define MinNoiseValue -0.864366

float InvLerp(float a, float b, float v);
void GetNoiseVal(int x, int y,struct RGB* dest[], NoiseObj* noise);
void InitNoise(NoiseObj* noise);

//struct osn_context* ctx;

unsigned char* pixels;

void InitNoise(NoiseObj* noise)
{
    open_simplex_noise(200, &noise->ctx);
    
    float max = 0;
    float min = 0;

    float amplitude = noise->amplitudeVal;
    float frequency = noise->frequencyVal;

    for (int i = 0; i < noise->octaves; i++)
    {
        max += (MaxNoiseValue * 2 - 1) * amplitude;
        min += (MinNoiseValue * 2 - 1) * amplitude;

        amplitude *= noise->persistance;
        frequency *= noise->lacunarity;
    }

    noise->maxNoiseHeight = max;
    noise->minNoiseHeight = min;
}

float GetSingleNoiseVal(int x, int y, struct RGB* dest, NoiseObj* noise)
{

    float amplitude = noise->amplitudeVal;
    float frequency = noise->frequencyVal;
    float noiseHeight = 0;

    for (int i = 0; i < noise->octaves; i++)
    {
        float sampleX = (x + 20) / noise->scale * frequency;
        float sampleY = (y - 40) / noise->scale * frequency;

        float noiseValue = open_simplex_noise2(ctx, sampleX, sampleY) * 2 - 1;

        noiseHeight += noiseValue * amplitude;
        amplitude *= noise->persistance;
        frequency *= noise->lacunarity;
    }

    float height = InvLerp(noise->minNoiseHeight, noise->maxNoiseHeight, noiseHeight);

    for (int i = 0; i < BiomeCount; i++)
    {
        if (height >= terrainRegions[i]->height)
        {
            struct RGB* color = (struct RGB*)malloc(sizeof(struct RGB));

            color->red = terrainRegions[i]->color->red;
            color->green = terrainRegions[i]->color->green;
            color->blue = terrainRegions[i]->color->blue;
            color->height = height;
            color->block = terrainRegions[i]->color->block;

            dest = color;
            break;
        }
    }

    return height;
}

void GetNoiseVal(int x, int y,struct RGB* dest[], NoiseObj* noise)
{

    float map[ChunkSize * ChunkSize];

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

                float noiseValue = open_simplex_noise2(ctx, sampleX, sampleY) * 2 - 1;

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

            for (int i = 0; i < BiomeCount; i++)
            {
                if (height >= terrainRegions[i]->height)
                {
                    struct RGB* color = (struct RGB*)malloc(sizeof(struct RGB));

                    color->red = terrainRegions[i]->color->red;
                    color->green = terrainRegions[i]->color->green;
                    color->blue = terrainRegions[i]->color->blue;
                    color->height = height;
                    color->block = terrainRegions[i]->color->block;

                    dest[(raw - y) * ChunkSize + (col - x)] = color;
                    break;
                }
            }
        }
    }
}

int chunksSize = ChunkSize * ChunkView * 2;


void DrawNoise(NoiseObj* noise)
{
    struct RGB* colours[ChunkSize * ChunkSize];
    
    for (int y = 0; y < chunksSize; y+=16) {
        for (int x = 0; x < chunksSize; x+= 16) {
            
            GetNoiseVal(x-ChunkSize*ChunkView, y-ChunkSize*ChunkView, colours, noise);

            for (int raw = 0; raw < ChunkSize; raw++) {
                for (int col = 0; col < ChunkSize; col ++) {
                    noise->noiseMap[col+x][raw + y] = colours[raw * ChunkSize + col];
                }
            }
            
        }
    }
    CreateBMP(noise);
}

float InvLerp(float a, float b, float v)
{
    return (v - a) / (b - a);
}