#include <cglm/cglm.h>
#include "../Generators/Chunk/Headers/ChunkGenerator.h"
#include "../Textures/Headers/DrawNoise.h"
#include "../Utils/Headers/FileUtils.h"

bool MovesCollisions(vec3 targetCameraPos)
{
    vec3 target = { targetCameraPos[0], targetCameraPos[1], targetCameraPos[2] };

    int x = round(target[0]);
    int z = round(target[2]);

    struct RGB* color = (struct RGB*)malloc(sizeof(struct RGB));

    int height = GetSingleNoiseVal(x, z, color) * ChunkHeight/2;


    return target[1] - 2.0f > height;//&& target[1] - 1.0f >= WaterLevel;
}