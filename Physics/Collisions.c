#include "Collisions.h"

#include <Generators/Noises/NoiseStruct.h>
#include <Textures/DrawNoise.h>

bool MovesCollisions(vec3 targetCameraPos)
{
    vec3 target = { targetCameraPos[0], targetCameraPos[1],
                    targetCameraPos[2] };

    int x = round((double)target[0]);
    int z = round((double)target[2]);

    float height = GetSingleNoiseVal(x, z, NULL, heightNoise) * ChunkHeight / 2;

    return target[1] - 2.0f > height; //&& target[1] - 1.0f >= WaterLevel;
}
