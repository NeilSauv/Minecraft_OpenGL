#include "Collisions.h"

#include <Generators/Chunk/ChunkGenerator.h>
#include <Generators/Noises/NoiseStruct.h>
#include <Player/Controller.h>
#include <Textures/DrawNoise.h>
#include <math.h>
#include <Textures/BlockDef.h>

bool MovesCollisions(vec3 targetCameraPos)
{
    // 1. On calcule la distance du joueur par rapport au centre de la planète
    float dist = glm_vec3_norm(targetCameraPos);
    if (dist == 0.0f)
        return false;

    // 2. On trouve la direction (longitude/latitude) où se trouve le joueur
    vec3 dir;
    glm_vec3_scale(targetCameraPos, 1.0f / dist, dir);

    // 3. On lit le bruit 3D exactement sous les pieds du joueur
    BlockInfoStruct blockInfo;
    float heightF =
        GetSingleNoiseVal3D(dir[0], dir[1], dir[2], &blockInfo, heightNoise);

    // 4. On calcule le rayon du sol à cet endroit (Planète 600m + Montagnes
    // 80m)
    float planetRadius = 600.0f;
    float maxMountainHeight = 80.0f;
    float terrainHeight = heightF * maxMountainHeight;

    if (terrainHeight < 0.0f)
        terrainHeight = 0.0f; // Surface de l'eau

    float groundRadius = planetRadius + terrainHeight;

    // Le joueur fait 2.25 unités de haut, on soustrait sa taille pour que ses
    // pieds touchent le sol "true" signifie qu'il n'y a pas de collision (on
    // peut bouger)
    return (dist - 2.25f) >= groundRadius;
}