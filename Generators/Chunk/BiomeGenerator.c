#include "Headers/BiomeGenerator.h"

unsigned char* biomeAtlas = 0;

void InitBiomeAtlas(void)
{
    biomeAtlas = ReadBPM("Textures/Pictures/biomeAtlas.bmp");
}
