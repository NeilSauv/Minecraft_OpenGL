#ifndef DRAW_NOISE
#define DRAW_NOISE

#include "../../Generators/Chunk/Headers/ChunkGenerator.h"
#include "ColorMap.h"

void DrawNoise(struct NoiseObj* noise);

void InitNoise(struct NoiseObj* noise);

void GetNoiseVal(int x, int y, struct RGB* dest[],struct NoiseObj* noise);

float GetSingleNoiseVal(int x, int y, struct RGB* dest,struct NoiseObj* noise);

extern struct RGB* noiseMap[ChunkSize * ChunkView * 2][ChunkSize * ChunkView * 2];


#endif // !DRAW_NOISE

