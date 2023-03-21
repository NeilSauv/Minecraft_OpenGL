#include <stdio.h>
#include <stdlib.h>

#include "../../Generators/Noises/Headers/NoisesHeaders.h"
#include "../../Textures/Headers/TextureHeaders.h"

RGB*** InitNoiseMap()
{
	RGB*** noiseMap = malloc(sizeof(RGB*) * ChunkView * ChunkView * 2);

	for (int i = 0; i < ChunkView * ChunkView * 2; i++)
	{
		noiseMap[i] = malloc(sizeof(RGB*) * ChunkSize * ChunkSize);

		for (int j = 0; j < ChunkSize * ChunkSize; j++)
		{
			RGB* rgb = malloc(sizeof(RGB));
			rgb->red = 0;
			rgb->green = 0;
			rgb->blue = 0;
			
			noiseMap[i][j] = rgb;
		}
	}

	return noiseMap;
}

BlockInfoStruct*** InitBlockInfoStruct()
{
	BlockInfoStruct*** blocks = calloc(ChunkView*ChunkView*2, sizeof(BlockInfoStruct**));

	for (int i = 0; i < ChunkView * ChunkView * 2; i++)
	{
		blocks[i] = calloc(ChunkSize * ChunkSize, sizeof(BlockInfoStruct*));

		for (int j = 0; j < ChunkSize * ChunkSize; j++)
		{
			struct BlockInfoStruct* block = malloc(sizeof(BlockInfoStruct));
			block->blockType = Air;
			block->height = 0;

			blocks[i][j] = block;
		}
	}

	return blocks;
}

void InitNoiseStruct()
{
	SimplexNoiseObj heightNoiseObj =
	{
			.name = TERRAIN,
			.ctx = malloc(sizeof(struct osn_context*)),
			.colorScheme = malloc(sizeof(ColorScheme)),
			.blocks = InitBlockInfoStruct(),
			.noiseMap = InitNoiseMap(),

			.scale = 200.0f,
			.octaves = 5,
			.persistance = 0.5f,
			.lacunarity = 2.0f,
			.amplitudeVal = 1.7f,
			.frequencyVal = 1.0f,

			.maxNoiseHeight = 0.864365f,
			.minNoiseHeight = -0.864366f
	};

	SimplexNoiseObj temperatureNoiseObj =
	{
		.name = BIOME_TEMP,
		.ctx = malloc(sizeof(struct osn_context*)),
		.colorScheme = malloc(sizeof(ColorScheme)),
		.blocks = InitBlockInfoStruct(),
		.noiseMap = InitNoiseMap(),

		.scale = 1.0f,
		.octaves = 5,
		.persistance = 0.5f,
		.lacunarity = 2.0f,
		.amplitudeVal = 1.7f,
		.frequencyVal = 1.0f,

		.maxNoiseHeight = 0.864365f,
		.minNoiseHeight = -0.864366f
	};

	heightNoise = &heightNoiseObj;
	temperatureNoise = &temperatureNoiseObj;
}
