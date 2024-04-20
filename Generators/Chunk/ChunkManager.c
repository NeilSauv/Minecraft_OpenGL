#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../Generators/Chunk/Headers/ChunkHeaders.h"
#include "../../Generators/Noises/Headers/NoisesHeaders.h"
#include "../../Player/Headers/PlayerHeaders.h"
#include "../../Utils/Headers/UtilsHeaders.h"
#include "../../Textures/Headers/TextureHeaders.h"

#define ChunkView 20

void DrawChunk();
int Modulo(int a, int b);
int ModuloZ(int a, int b, int i);
int DivideDestroy(int n);

const int chunks = ChunkView* ChunkView * 4;
int count = 0;

int offsetX = 0;
int offsetZ = 0;

void GenerateChunks()
{
    glGenVertexArrays(chunks,(GLuint *) &VAO); //Generate vertex array

    glGenBuffers(chunks,(GLuint *) &VBO); //Generate buffer
    glGenBuffers(chunks,(GLuint *) &EBO);
    glGenBuffers(chunks,(GLuint *) &faceVBO);
    glGenBuffers(chunks,(GLuint *) &instanceVBO);
    glGenBuffers(chunks,(GLuint *) &renderVBO);

    for (int x = -ChunkView; x < ChunkView; x += 1)
    {
        for (int z = -ChunkView; z < ChunkView; z += 1)
        {
            CreateChunk(x*ChunkSize, -ChunkHeight / 2, z * ChunkSize, count, false);
            count++;
        }
    }
}

void DrawChunk()
{
    Draw(count);
}

void Update()
{
    bool xUpdate = false;
    bool zUpdate = false;

    int addAxisX = 0;
    int addAxisZ = 0;

    int ChunkLength = ChunkView * 2;

    //X Axis
    if (cameraPos[0] > (offsetX+1) * ChunkSize)
    {
        offsetX++;
        addAxisX = -1;
        xUpdate = true;
    }
    else if(cameraPos[0] < (offsetX-1) * ChunkSize)
    {
        offsetX--;
        addAxisX = 0;
        xUpdate = true;
    }
    // Z Axis
    if (cameraPos[2] < (offsetZ -1) * ChunkSize)
    {
        offsetZ--;
        zUpdate = true;
        addAxisZ = 0;
    }
    if (cameraPos[2] > (offsetZ+ 1) * ChunkSize)
    {
        offsetZ++;
        zUpdate = true;
        addAxisZ = -1;
    }
    
    if (xUpdate)
    {

        xUpdate = false;
        for (int i = -ChunkView; i < ChunkView; i++)
        {
            int index = i + ChunkView;
            int sign = (addAxisX * (-2) - 1);
            int xOffset = offsetX + addAxisX;
            CreateChunk((float)(sign * ChunkView + xOffset) * ChunkSize, (float) - ChunkHeight / 2, (float)(i + offsetZ) * ChunkSize, Modulo(Modulo(index + offsetZ, ChunkLength) + ChunkLength * xOffset, count), false);
        }
    }

    if (zUpdate)
    {
        zUpdate = false;
        for (int i = -ChunkView; i < ChunkView; i++)
        {
            int index = i + ChunkView;
            int sign = (addAxisZ * (-2) - 1);
            int zOffset = offsetZ + addAxisZ;
            CreateChunk((float)(i+offsetX) * ChunkSize, (float) - ChunkHeight / 2, (float)(sign * ChunkView + zOffset) * ChunkSize, ModuloZ(Modulo(index + offsetX, ChunkLength) * ChunkLength + zOffset, ChunkLength, Modulo(index + offsetX, ChunkLength)), false);
        }
    }

}

void UpdateDestroyed(int chunkIndex)
{

    int i = 0;
    for (int x = -ChunkView; x < ChunkView; x += 1)
    {
        for (int z = -ChunkView; z < ChunkView; z += 1)
        {
            if (chunkIndex == i)
            {
                offsetX = 0;
                offsetZ = 0;
                CreateChunk((float)(x + offsetX) * ChunkSize, (float) - ChunkHeight / 2, (float)(z + offsetZ) * ChunkSize, i, true);
            }
            i++;
        }
    }
}

int DivideDestroy(int n)
{
    if (n < 0)
        return -(n / ChunkSize) - ChunkSize;
    
    return n / ChunkSize;
}

int Modulo(int a, int b)
{
    int r = a % b;
    return r < 0 ? r+b : r;
}

int ModuloZ(int a, int b, int i)
{
    int r = a - i * b;
    r %= b;
    return r < 0 ? r+b + i*b : r + i * b;
}


void ClearChunk()
{
    glDeleteVertexArrays(count,(GLuint *) &VAO);


    glDeleteBuffers(count,(GLuint *) &VBO);
    glDeleteBuffers(count,(GLuint *) &EBO);
    glDeleteBuffers(count,(GLuint *) &VAO);
    glDeleteBuffers(count,(GLuint *) &faceVBO);
    glDeleteBuffers(count,(GLuint *) &instanceVBO);
    glDeleteBuffers(count,(GLuint *) &renderVBO);
}
