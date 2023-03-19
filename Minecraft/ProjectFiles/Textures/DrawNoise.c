#include <stdio.h>
#include <math.h>

#include "../Utils/Headers/FileUtils.h"
#include "../Generators/Noises/Headers/SimplexNoise.h"
#include "../Generators/Chunk/Headers/ChunkGenerator.h"
#include "../Generators/Chunk/Headers/ChunkManager.h"
#include "Headers/ColorMap.h"
#include "Headers/BitmapCreator.h"

#define MaxNoiseValue 0.864365
#define MinNoiseValue -0.864366

void SetValueBW(int x, int y, int val);
void SetValueColor(int x, int y, struct RGB* color);
int GetValueBW(int x, int y); 
float InvLerp(float a, float b, float v);
void CreateImage();
void GetNoiseVal(int x, int y,struct RGB* dest[]);
void InitNoise();

//struct osn_context* ctx;

unsigned char* pixels;
struct RGB* noiseMap[ChunkSize*ChunkView*2][ChunkSize * ChunkView * 2];

float scale = 200;
int octaves = 5;
float persistance = 0.5;
float lacunarity = 2;
float amplitudeVal = 1.7;
float frequencyVal = 1;

float maxNoiseHeight = FLT_MIN;
float minNoiseHeight = FLT_MAX;

void InitNoise()
{
    open_simplex_noise(200, &ctx);
    
    float max = 0;
    float min = 0;

    float amplitude = amplitudeVal;
    float frequency = frequencyVal;

    for (int i = 0; i < octaves; i++)
    {
        max += (MaxNoiseValue * 2 - 1) * amplitude;
        min += (MinNoiseValue * 2 - 1) * amplitude;

        amplitude *= persistance;
        frequency *= lacunarity;
    }

    maxNoiseHeight = max;
    minNoiseHeight = min;

}

float GetSingleNoiseVal(int x, int y, struct RGB* dest)
{

    float amplitude = amplitudeVal;
    float frequency = frequencyVal;
    float noiseHeight = 0;

    for (int i = 0; i < octaves; i++)
    {
        float sampleX = (x + 20) / scale * frequency;
        float sampleY = (y - 40) / scale * frequency;

        float noiseValue = open_simplex_noise2(ctx, sampleX, sampleY) * 2 - 1;

        noiseHeight += noiseValue * amplitude;
        amplitude *= persistance;
        frequency *= lacunarity;
    }

    float height = InvLerp(minNoiseHeight, maxNoiseHeight, noiseHeight);

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

void GetNoiseVal(int x, int y,struct RGB* dest[])
{

    float map[ChunkSize * ChunkSize];

    for (int raw = y; raw < ChunkSize + y; raw++) {
        for (int col = x; col < ChunkSize + x; col++) {

            float halfHeight = 0;
            float halfWidth = 0;

            float amplitude = amplitudeVal;
            float frequency = frequencyVal;
            float noiseHeight = 0;

            for (int i = 0; i < octaves; i++)
            {
                float sampleX = (col-halfWidth + 20) / scale * frequency;
                float sampleY = (raw-halfHeight -40) / scale * frequency;

                float noiseValue = open_simplex_noise2(ctx, sampleX, sampleY) * 2 - 1;

                noiseHeight += noiseValue * amplitude;
                amplitude *= persistance;
                frequency *= lacunarity;
            }

            map[(raw - y) * ChunkSize + (col - x)] = noiseHeight;
        }
    }

    for (int raw = y; raw < ChunkSize + y; raw++) {
        for (int col = x; col < ChunkSize + x; col++) {
            float height = InvLerp(minNoiseHeight, maxNoiseHeight, map[(raw - y) * ChunkSize + (col - x)]);

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


void DrawNoise()
{
    struct RGB* colours[ChunkSize * ChunkSize];
    
    for (int y = 0; y < chunksSize; y+=16) {
        for (int x = 0; x < chunksSize; x+= 16) {
            
            GetNoiseVal(x-ChunkSize*ChunkView, y-ChunkSize*ChunkView, colours);

            for (int raw = 0; raw < ChunkSize; raw++) {
                for (int col = 0; col < ChunkSize; col ++) {
                    noiseMap[col+x][raw + y] = colours[raw * ChunkSize + col];
                }
            }
            
        }
    }
    CreateBMP();
}

float InvLerp(float a, float b, float v)
{
    return (v - a) / (b - a);
}