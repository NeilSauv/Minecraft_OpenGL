#include "BiomeGenerator.h"

#include <Generators/Noises/NoiseStruct.h>
#include <Textures/BitmapCreator.h>
#include <Textures/TextureSet.h>

unsigned char* biomeAtlas = 0;

void InitBiomeAtlas(void)
{
    biomeAtlas = ReadBPM("Textures/Pictures/biomeAtlas.bmp");
}

void BiomeSetup(struct SimplexNoiseObj* heightNoise, struct SimplexNoiseObj* temperatureNoise, struct SimplexNoiseObj* rainingNoise)
{

    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            int temperature = (float)(temperatureNoise->noiseMap[y][x]->height + 1) / 2 * 250;
            int raining = (float)(rainingNoise->noiseMap[y][x]->height + 1) / 2 * 250;


            heightNoise->noiseMap[y][x]->blue = biomeAtlas[temperature * 3 * 250 + raining * 3+2];
            heightNoise->noiseMap[y][x]->green = biomeAtlas[temperature * 3 * 250 + raining * 3+1];
            heightNoise->noiseMap[y][x]->red = biomeAtlas[temperature * 3 * 250 + raining * 3+3];
        }
    }
}
