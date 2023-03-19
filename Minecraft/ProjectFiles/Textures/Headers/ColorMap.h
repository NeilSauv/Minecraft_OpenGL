#ifndef COLOR_MAP
#define COLOR_MAP

#include "Block.h"

#define BiomeCount 5

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

extern struct terrainType* terrainRegions[BiomeCount];

void AddRegion(char name[], float height, int red, int green, int blue);

void InitTerrainRegions();

void ClearBiomes();

#endif // !COLOR_MAP
