#include "ChunkGenerator.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Generators/Noises/NoiseStruct.h>
#include <Textures/DrawNoise.h>
#include <Textures/ColorMap.h>
#include <Textures/BlockDef.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

float chunkVertices[VERTICES_PER_CHUNK * 9];

unsigned int VBO[ChunkView * ChunkView * 4];
unsigned int VAO[ChunkView * ChunkView * 4];
unsigned int EBO[ChunkView * ChunkView * 4]; // On l'ignore

// Une petite structure pratique pour stocker nos points 3D
typedef struct
{
    float x, y, z, h;
} Point3D;

// Fonction qui calcule la position 3D d'un sommet sur la planète
Point3D GetPlanetPoint(int gridX, int gridZ)
{
    float planetRadius = 600.0f;
    float angleScale = 0.005f;
    float maxMountainHeight = 80.0f;

    float theta = gridX * angleScale;
    float phi = gridZ * angleScale;

    // Direction pure depuis le centre
    float nx = cos(phi) * cos(theta);
    float ny = sin(phi);
    float nz = cos(phi) * sin(theta);

    BlockInfoStruct blockInfo;
    float heightF = GetSingleNoiseVal3D(nx, ny, nz, &blockInfo, heightNoise);
    float height = heightF * maxMountainHeight;

    if (height < 0.0f)
        height = 0.0f; // Niveau de l'eau

    Point3D p;
    float totalRadius = planetRadius + height;
    p.x = nx * totalRadius;
    p.y = ny * totalRadius;
    p.z = nz * totalRadius;
    p.h =
        heightF; // On garde la hauteur pure pour calculer la couleur plus tard
    return p;
}

// Fonction pour ajouter un sommet au tableau final
void AddVertex(Point3D p, float r, float g, float b, float nx, float ny,
               float nz, int *index)
{
    chunkVertices[(*index)++] = p.x;
    chunkVertices[(*index)++] = p.y;
    chunkVertices[(*index)++] = p.z;
    chunkVertices[(*index)++] = r;
    chunkVertices[(*index)++] = g;
    chunkVertices[(*index)++] = b;
    // On ajoute la normale mathématique !
    chunkVertices[(*index)++] = nx;
    chunkVertices[(*index)++] = ny;
    chunkVertices[(*index)++] = nz;
}

void CreateChunk(int xAxis, int yAxis, int zAxis, int i, bool destroyBlock)
{
    if (yAxis != 0)
        return;

    int vertexIndex = 0;

    for (int z = 0; z < ChunkSize; z++)
    {
        for (int x = 0; x < ChunkSize; x++)
        {
            int wX = xAxis + x;
            int wZ = zAxis + z;

            // On calcule les 4 coins de notre case
            Point3D p00 = GetPlanetPoint(wX, wZ);
            Point3D p10 = GetPlanetPoint(wX + 1, wZ);
            Point3D p01 = GetPlanetPoint(wX, wZ + 1);
            Point3D p11 = GetPlanetPoint(wX + 1, wZ + 1);

            float v1x = p01.x - p00.x;
            float v1y = p01.y - p00.y;
            float v1z = p01.z - p00.z;
            float v2x = p10.x - p00.x;
            float v2y = p10.y - p00.y;
            float v2z = p10.z - p00.z;
            float n1x = v1y * v2z - v1z * v2y;
            float n1y = v1z * v2x - v1x * v2z;
            float n1z = v1x * v2y - v1y * v2x;

            // On s'assure que la normale pointe vers l'extérieur de la planète
            if (n1x * p00.x + n1y * p00.y + n1z * p00.z < 0)
            {
                n1x = -n1x;
                n1y = -n1y;
                n1z = -n1z;
            }

            float avgH1 = (p00.h + p01.h + p10.h) / 3.0f;
            BlockInfoStruct b1;
            b1.height = avgH1;
            RGB *c1 = GetBlockColor(&b1, heightNoise);
            float r1 = 1.0f, g1 = 1.0f, b1_c = 1.0f;
            if (c1)
            {
                r1 = c1->red / 255.0f;
                g1 = c1->green / 255.0f;
                b1_c = c1->blue / 255.0f;
            }

            AddVertex(p00, r1, g1, b1_c, n1x, n1y, n1z, &vertexIndex);
            AddVertex(p01, r1, g1, b1_c, n1x, n1y, n1z, &vertexIndex);
            AddVertex(p10, r1, g1, b1_c, n1x, n1y, n1z, &vertexIndex);

            // --- TRIANGLE 2 (p10, p01, p11) ---
            float v3x = p01.x - p10.x;
            float v3y = p01.y - p10.y;
            float v3z = p01.z - p10.z;
            float v4x = p11.x - p10.x;
            float v4y = p11.y - p10.y;
            float v4z = p11.z - p10.z;
            float n2x = v3y * v4z - v3z * v4y;
            float n2y = v3z * v4x - v3x * v4z;
            float n2z = v3x * v4y - v3y * v4x;

            if (n2x * p10.x + n2y * p10.y + n2z * p10.z < 0)
            {
                n2x = -n2x;
                n2y = -n2y;
                n2z = -n2z;
            }

            float avgH2 = (p10.h + p01.h + p11.h) / 3.0f;
            BlockInfoStruct b2;
            b2.height = avgH2;
            RGB *c2 = GetBlockColor(&b2, heightNoise);
            float r2 = 1.0f, g2 = 1.0f, b2_c = 1.0f;
            if (c2)
            {
                r2 = c2->red / 255.0f;
                g2 = c2->green / 255.0f;
                b2_c = c2->blue / 255.0f;
            }

            AddVertex(p10, r2, g2, b2_c, n2x, n2y, n2z, &vertexIndex);
            AddVertex(p01, r2, g2, b2_c, n2x, n2y, n2z, &vertexIndex);
            AddVertex(p11, r2, g2, b2_c, n2x, n2y, n2z, &vertexIndex);
        }
    }

    glBindVertexArray(VAO[i]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(chunkVertices), chunkVertices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                          (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Draw(int i)
{
    int n = 0;
    while (n < i)
    {
        glBindVertexArray(VAO[n]);
        // Magie : on utilise glDrawArrays au lieu de glDrawElements,
        // car on n'utilise plus l'EBO (Index Buffer) !
        glDrawArrays(GL_TRIANGLES, 0, VERTICES_PER_CHUNK);
        n++;
    }
}