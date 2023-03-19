#include "../../Player/Headers/Destroy.h"
#include "Headers/ChunkManager.h"
#include "Headers/ChunkGenerator.h"

#include "../../Utils/Headers/FileUtils.h"
#include <math.h>


#define RegionCount 4
#define RegionSize 32

int xOffset = 0;
int zOffset = 0;

struct regionList
{
	int x;
	int z;
	struct destroyList* destroyList[RegionSize*RegionSize];
};


struct regionList* regions[RegionCount][RegionCount];
struct regionList* actualRegion;
struct destroyList* destroyList;


float RoundRegion(float pos);

void InitRegion()
{
	for (int x = 0; x < RegionCount; x++)
	{
		for (int z = 0; z < RegionCount; z++)
		{
			struct regionList *region = (struct regionList*)malloc(sizeof(struct regionList));
			region->x = x;
			region->z = z;
			for (int i = 0; i < RegionSize * RegionSize; i++)
			{
				region->destroyList[i] = NULL;
			}
			regions[x][z] = region;
		}
	}
}

int AddBlockState(float pos[])
{
	int renderIndex = (Modulo(pos[0], ChunkSize) * ChunkSize + Modulo(pos[2] , ChunkSize)) * ChunkHeight + pos[1] + ChunkHeight/2;
	
	float position[] = {(float) pos[0], (float) pos[2]};
	position[0] -= xOffset * RegionSize * ChunkSize;
	position[1] -= zOffset * RegionSize * ChunkSize;

	// Region coords
	position[0] = RoundRegion(position[0] / (RegionSize * ChunkSize));
	position[1] = RoundRegion(position[1] / (RegionSize * ChunkSize));

	int chunkRegion = RegionSize * ChunkSize;

	int xAdd = 0;
	int zAdd = 0;

	if (pos[0] < 0)
		xAdd = 1;
	if (pos[2] < 0)
		zAdd = 1;

	int x = RoundRegion((float)((int) pos[0] % chunkRegion) / (ChunkSize + xAdd));
	int z = RoundRegion((float)((int) pos[2] % chunkRegion) / (ChunkSize + zAdd));

	int chunkIndex = (x + ChunkView) * ChunkView * 2 + z + ChunkView;

	struct regionList* actualRegion = regions[(int)position[0] + 1][(int)position[1] + 1];

	if (actualRegion->destroyList[chunkIndex] == NULL)
	{
		destroyList = (struct destroyList*)malloc(sizeof(struct destroyList));
		destroyList->data = renderIndex;
		destroyList->next = NULL;
		actualRegion->destroyList[chunkIndex] = destroyList;
	}
	else
	{
		Add(actualRegion->destroyList[chunkIndex], renderIndex);
	}

	return chunkIndex;
} 

bool DestroyPossible(int pos[])
{

	int renderIndex = (Modulo(pos[0], ChunkSize) * ChunkSize + Modulo(pos[2], ChunkSize)) * ChunkHeight + pos[1] + ChunkHeight / 2;

	float position[] = { (float)pos[0], (float)pos[2] };
	position[0] -= xOffset * RegionSize * ChunkSize;
	position[1] -= zOffset * RegionSize * ChunkSize;

	// Region coords
	position[0] = RoundRegion(position[0] / (RegionSize * ChunkSize));
	position[1] = RoundRegion(position[1] / (RegionSize * ChunkSize));

	int chunkRegion = RegionSize * ChunkSize;

	int xAdd = 0;
	int zAdd = 0;

	if (pos[0] < 0)
		xAdd = 1;
	if (pos[2] < 0)
		zAdd = 1;

	int x = RoundRegion((float)(pos[0] % chunkRegion) / (ChunkSize + xAdd));
	int z = RoundRegion((float)(pos[2] % chunkRegion) / (ChunkSize + zAdd));

	int chunkIndex = (x + ChunkView) * ChunkView * 2 + z + ChunkView;

	struct regionList* actualRegion = regions[(int)position[0] + 1][(int)position[1] + 1];

	if (actualRegion->destroyList[chunkIndex] == NULL)
		return true;
	else
	{
		destroyList = actualRegion->destroyList[chunkIndex];

		while (destroyList != NULL)
		{
			if (destroyList->data == renderIndex)
				return false;

			destroyList = destroyList->next;
		}
	}

	return true;
}

void FreeStruct()
{
	for (int x = 0; x < RegionCount; x++)
	{
		for (int z = 0; z < RegionCount; z++)
		{
			free(regions[x][z]);
		}
	}
	free(actualRegion);
	free(destroyList);
}

int GetRegion(int pos)
{
	return RoundRegion(pos / (RegionSize * ChunkSize));
}

float RoundRegion(float pos)
{
	if (pos >= 0)
		return (int)pos;

	return (int)pos -1;
}