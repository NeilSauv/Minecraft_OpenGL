#ifndef BIOME_GENERATOR
#define BIOME_GENERATOR

#include <Generators/Noises/NoiseStruct.h>

extern unsigned char *biomeAtlas;

void InitBiomeAtlas(void);

void BiomeSetup(struct SimplexNoiseObj *heightNoise,
                struct SimplexNoiseObj *temperatureNoise,
                struct SimplexNoiseObj *rainingNoise);
#endif
