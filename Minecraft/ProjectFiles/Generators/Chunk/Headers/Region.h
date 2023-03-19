#ifndef REGION
#define REGION

#define RegionCount 4
#define RegionSize 32

#include <stdbool.h>


struct regionList
{
	int x;
	int z;
	struct destroyList* destroyList[RegionSize * RegionSize];
};

extern struct regionList* regions[RegionCount][RegionCount];

int GetRegion(int pos);

void InitRegion();

int AddBlockState(float pos[]);

float RoundRegion(float pos);

bool DestroyPossible(int pos[]);

void FreeStruct();

#endif // !REGION
