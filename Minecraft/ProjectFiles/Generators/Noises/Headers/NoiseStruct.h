#ifndef NOISE_STRUCT
#define NOISE_STRUCT

#include "../../Chunk/Headers/ChunkGenerator.h"

typedef struct NoiseObj 
{
	enum NoiseName name;
	struct osn_context* ctx;
	struct RGB* noiseMap[ChunkSize * ChunkView * 2][ChunkSize * ChunkView * 2];

	float scale;
	int octaves;
	float persistance;
	float lacunarity;
	float amplitudeVal;
	float frequencyVal;

	float maxNoiseHeight;
	float minNoiseHeight;
}NoiseObj;

extern enum NoiseName
{
	TERRAIN,
	BIOME_TEMP,
	BIOME_HUM
};

extern NoiseObj terrain =
{
	.name = TERRAIN,
	.ctx = NULL,
	.noiseMap = NULL,

	.scale = 200.0f,
	.octaves = 5,
	.persistance = 0.5f,
	.lacunarity = 2.0f,
	.amplitudeVal = 1.7f,
	.frequencyVal = 1.0f,

	.maxNoiseHeight = 0.864365f,
	.minNoiseHeight = -0.864366f
};


#endif