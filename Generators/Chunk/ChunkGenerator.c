#include "ChunkGenerator.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <Generators/Noises/NoiseStruct.h>
#include <Textures/DrawNoise.h>
#include <cglm/cglm.h>
#include <stdlib.h>

#include "Utils/FileUtils.h"

float translations[ChunkSize * ChunkSize * ChunkHeight][3];
int rendering[ChunkSize * ChunkSize * ChunkHeight * 4];

unsigned int VBO[ChunkView * ChunkView * 4];
unsigned int VAO[ChunkView * ChunkView * 4];
unsigned int EBO[ChunkView * ChunkView * 4];

unsigned int faceVBO[ChunkView * ChunkView * 4];
unsigned int instanceVBO[ChunkView * ChunkView * 4];
unsigned int renderVBO[ChunkView * ChunkView * 4];

void ChangeFaces(bool single);

float texCoords[] = {
    -0.5, 0.5,  0.5,  0.5,  -0.5, -0.5, 0.5,  -0.5,

    -0.5, 0.5,  0.5,  0.5,  -0.5, -0.5, 0.5,  -0.5,

    0.5,  -0.5, -0.5, -0.5, 0.5,  0.5,  -0.5, 0.5,

    -0.5, 0.5,  0.5,  0.5,  -0.5, -0.5, 0.5,  -0.5,

    -0.5, 0.5,  0.5,  0.5,  -0.5, -0.5, 0.5,  -0.5,

    -0.5, 0.5,  0.5,  0.5,  -0.5, -0.5, 0.5,  -0.5,
};

float faces[] = {
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2,
};

bool faceSingle = false;

int renderBlock[ChunkSize * ChunkSize * ChunkHeight * 3];

float vertices[] = {

    // Top
    -0.5,
    0.5,
    0.5,
    0,
    1,
    0.5,
    0.5,
    0.5,
    1,
    1,
    -0.5,
    0.5,
    -0.5,
    0,
    0,
    0.5,
    0.5,
    -0.5,
    1,
    0,

    // Front
    -0.5,
    -0.5,
    -0.5,
    0,
    1,
    -0.5,
    -0.5,
    0.5,
    1,
    1,
    -0.5,
    0.5,
    -0.5,
    0,
    0,
    -0.5,
    0.5,
    0.5,
    1,
    0,

    // Left
    -0.5,
    0.5,
    -0.5,
    1,
    0,
    0.5,
    0.5,
    -0.5,
    0,
    0,
    -0.5,
    -0.5,
    -0.5,
    1,
    1,
    0.5,
    -0.5,
    -0.5,
    0,
    1,

    // Back
    0.5,
    -0.5,
    0.5,
    0,
    1,
    0.5,
    -0.5,
    -0.5,
    1,
    1,
    0.5,
    0.5,
    0.5,
    0,
    0,
    0.5,
    0.5,
    -0.5,
    1,
    0,

    // Right
    -0.5,
    -0.5,
    0.5,
    0,
    1,
    0.5,
    -0.5,
    0.5,
    1,
    1,
    -0.5,
    0.5,
    0.5,
    0,
    0,
    0.5,
    0.5,
    0.5,
    1,
    0,

    // Bottom
    -0.5,
    -0.5,
    -0.5,
    0,
    1,
    0.5,
    -0.5,
    -0.5,
    1,
    1,
    -0.5,
    -0.5,
    0.5,
    0,
    0,
    0.5,
    -0.5,
    0.5,
    1,
    0,
};

unsigned int indices[] = {
    0,  1,  2,  2,  1,  3,

    4,  5,  6,  6,  5,  7,

    8,  9,  10, 10, 9,  11,

    12, 13, 14, 14, 13, 15,

    16, 17, 18, 18, 17, 19,

    20, 21, 22, 22, 21, 23,
};

void CreateChunk(int xAxis, int yAxis, int zAxis, int i, bool destroyBlock)
{
    // Vertices
    glBindVertexArray(VAO[i]); // Bind vertex array
    glBindBuffer(GL_ARRAY_BUFFER, VBO[i]); // Bind one buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                 GL_STATIC_DRAW); // Store data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0); // vertex buffer interpretation
    glEnableVertexAttribArray(0);

    // Texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);
    if (destroyBlock)
        printf("Destroy\n");
    /*
       if (destroyBlock)
       {
       int regionX = RoundRegion(xAxis / (RegionSize * ChunkSize)) + 1;
       int regionZ = RoundRegion(zAxis / (RegionSize * ChunkSize)) + 1;

       struct regionList* region = regions[regionX][regionZ];

       struct DestroyList* destroy = region->destroyList[i];

       while (destroy != NULL)
       {
       rendering[destroy->data*4] = 2;
       destroy = destroy->next;
       }
       }
       */
    BlockInfoStruct **blocks =
        malloc(ChunkSize * ChunkSize * sizeof(BlockInfoStruct *));

    GetNoiseMap(xAxis, zAxis, heightNoise, blocks);

    int renderCount = 0;

    for (int x = 0; x < ChunkSize; x++)
    {
        for (int z = 0; z < ChunkSize; z++)
        {
            int height = blocks[z * ChunkSize + x]->height * ChunkHeight;
            bool blockNotFound = true;
            // bool waterNotFound = true;

            for (int y = ChunkHeight; y > 0 && blockNotFound; y--)
            {
                // int index = (x * ChunkSize + z) * ChunkHeight + y;

                if (y + yAxis == height || y + yAxis == WaterLevel)
                {
                    BlockTypeEnum block = blocks[z * ChunkSize + x]->blockType;

                    if (y + yAxis == WaterLevel)
                    {
                        block = Water;
                        // blockNotFound = false;
                    }
                    else if (y + yAxis == height)
                    {
                        blockNotFound = false;
                    }

                    translations[renderCount][0] = x + xAxis;
                    translations[renderCount][1] = y + yAxis;
                    translations[renderCount][2] = z + zAxis;

                    rendering[(renderCount) * 4] = block;

                    renderCount++;
                }
            }
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, faceVBO[i]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void *)0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO[i]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * renderCount, &translations[0],
                 GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);

    glBindBuffer(GL_ARRAY_BUFFER, renderVBO[i]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(int) * renderCount * 4, &rendering[0],
                 GL_STATIC_DRAW);
    glVertexAttribPointer(4, 4, GL_INT, GL_FALSE, sizeof(int) * 4, (void *)0);
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);
}

int multFaces[] = { 0, 1, 1, 1, 1, 2 };

int singleFaces[] = { 0, 0, 0, 0, 0, 0 };

void ChangeFaces(bool single)
{
    if (single)
    {
        for (int i = 0; i < 4 * 6; i++)
            faces[i] = 0;
    }
    else
    {
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                faces[i * 4 + j] = multFaces[i];
            }
        }
    }

    faceSingle = !faceSingle;
}

void Draw(int i)
{
    int n = 0;
    while (n < i)
    {
        glBindVertexArray(VAO[n]);
        glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0,
                                (ChunkSize) * (ChunkSize));
        n++;
    }
}
