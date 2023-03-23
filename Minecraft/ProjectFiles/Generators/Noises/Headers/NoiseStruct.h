#ifndef NOISE_STRUCT
#define NOISE_STRUCT

#include "../../Chunk/Headers/ChunkGenerator.h"
#include "../../../Textures/Headers/ColorMap.h"
#include "../../../Generators/Noises/Headers/NoisesHeaders.h"

typedef enum NoiseNameEnum
{
	TERRAIN,
	BIOME_TEMP,
	BIOME_HUM
}NoiseNameEnum;

struct SimplexNoiseObj
{
	enum NoiseNameEnum name;
	struct osn_context* ctx;
	struct ColorScheme* colorScheme;
	struct BlockInfoStruct*** blocks;
	struct RGBH*** noiseMap;
	float scale;
	int octaves;
	float persistance;
	float lacunarity;
	float amplitudeVal;
	float frequencyVal;
	float maxNoiseHeight;
	float minNoiseHeight;
};

typedef struct SimplexNoiseObj SimplexNoiseObj;

SimplexNoiseObj* heightNoise;
SimplexNoiseObj* temperatureNoise;
SimplexNoiseObj* rainingNoise;

int blockPatterns[42];

void InitNoiseStruct();

void CompleteNoiseMap(struct SimplexNoiseObj* noise);

void InitBlockPattern(struct SimplexNoiseObj* noise);

#endif