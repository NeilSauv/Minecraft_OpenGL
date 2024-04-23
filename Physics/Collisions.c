#include "Collisions.h"

#include <Generators/Noises/NoiseStruct.h>
#include <stdio.h>

#include "Generators/Chunk/ChunkGenerator.h"
#include "Player/Controller.h"
#include "Textures/DrawNoise.h"
static int AddAbsolute(float val, float add, int camPos)
{
    float center = 0.5f;
    if (cameraPos[camPos] < 0)
        center = -0.5f;

    if (cameraPos[camPos] - val < 0.f)
        return val + add + center;
    if (cameraPos[camPos] == val)
        return val + center;
    return val - add + center;
}

bool MovesCollisions(vec3 targetCameraPos)
{
    int height = GetSingleNoiseVal(AddAbsolute(targetCameraPos[0], 0.25f, 0),
                                   AddAbsolute(targetCameraPos[2], 0.25f, 2),
                                   NULL, heightNoise)
        * ChunkHeight;
    printf("%f\n", cameraPos[0]);

    return (targetCameraPos[1] - 2.25f) >= height
        && (targetCameraPos[1] - 2.25f) >= WaterLevel;
}
