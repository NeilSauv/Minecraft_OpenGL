#include <stdlib.h>
#include "Headers/Block.h"

#define BiomeCount 5

void AddRegion(char name[], float height, int red, int green, int blue, enum Block block);

struct RGB {
	unsigned int red;
	unsigned int green;
	unsigned int blue;
	enum Block block;
	float height;
};

struct terrainType {
	char* name;
	float height;
	struct RGB* color;
};

struct terrainType* terrainRegions[BiomeCount];
int i = 0;

void AddRegion(char name[], float height, int red, int green, int blue, enum Block block)
{
	struct RGB* color = (struct RGB*)malloc(sizeof(struct RGB));

	color->red = red;
	color->green = green;
	color->blue = blue;
	color->height = height;
	color->block = block;

	struct terrainType* terrain = (struct terrainType*)malloc(sizeof(struct terrainType));
	terrain->height = height;
	terrain->name = name;
	terrain->color = color;

	terrainRegions[i] = terrain;
	i++;
}

void InitTerrainRegions()
{
	AddRegion("Snow", 0.8f, 255, 255, 255, Snow);
	AddRegion("Stone", 0.7f, 163, 163, 163, Stone);
	AddRegion("Grass", 0.54f, 0, 255, 0, Grass);
	AddRegion("Sand", 0.5f, 200, 255, 0, Sand);
	AddRegion("Water", 0.0f, 0, 0, 255, Water);
}

void ClearBiomes()
{
	while (i > 0)
	{
		free(terrainRegions[i-1]->color);
		free(terrainRegions[i-1]);
		i--;
	}
}