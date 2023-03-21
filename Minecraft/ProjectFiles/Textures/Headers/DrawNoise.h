#ifndef DRAW_NOISE
#define DRAW_NOISE

#include "../../Generators/Chunk/Headers/ChunkHeaders.h"
#include "../../Textures/Headers/TextureHeaders.h"
#include "../../Generators/Noises/Headers/NoisesHeaders.h"

void DrawNoise(struct SimplexNoiseObj* noise);

void InitNoise(struct SimplexNoiseObj* noise);

void GetNoiseMap(int x, int y,struct SimplexNoiseObj* noise, struct BlockInfoStruct** blocks);

float GetSingleNoiseVal(int x, int y, struct BlockInfoStruct* block, struct SimplexNoiseObj* noise);


#endif // !DRAW_NOISE

