#ifndef DRAW_NOISE
#define DRAW_NOISE

#include <Generators/Noises/SimplexNoise.h>

void DrawNoise(struct SimplexNoiseObj *noise, char *name);

void GetNoiseMap(int x, int y, struct SimplexNoiseObj *noise,
                 struct BlockInfoStruct **blocks);

float GetSingleNoiseVal(int x, int y, struct BlockInfoStruct *block,
                        struct SimplexNoiseObj *noise);

#endif // !DRAW_NOISE
