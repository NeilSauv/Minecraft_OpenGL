#ifndef DRAW_NOISE
#define DRAW_NOISE

#include "../../Generators/Chunk/Headers/ChunkGenerator.h"
#include "ColorMap.h"

void SetValBW(int x, int y, int val, int size);

void CreateImage();

void CreateColorImage();

void DrawNoise();

void InitNoise();

void GetNoiseVal(int x, int y, struct RGB* dest[]);

float GetSingleNoiseVal(int x, int y, struct RGB* dest);

extern struct RGB* noiseMap[ChunkSize * ChunkView * 2][ChunkSize * ChunkView * 2];


#endif // !DRAW_NOISE

