#ifndef REGION
#define REGION

#define RegionCount 4
#define RegionSize 20

#include <stdbool.h>

struct regionList
{
    int x;
    int z;
    struct DestroyList* destroyList[RegionSize * RegionSize];
};

extern struct regionList* regions[RegionCount][RegionCount];

int GetRegion(int pos);

void InitRegion();

int AddBlockState(float pos[]);

int RoundRegion(float pos);

bool DestroyPossible(int pos[]);

void FreeStruct();

#endif // !REGION
