#include "ChunkManager.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Generators/Chunk/ChunkGenerator.h>
#include <Player/Controller.h>
#include <math.h>

void DrawChunk();
int Modulo(int a, int b);
int ModuloZ(int a, int b, int i);
int DivideDestroy(int n);

const int chunks = ChunkView * ChunkView * 4;
int count = 0;

int offsetX = 0;
int offsetZ = 0;
int currentFace = 0;

void GenerateChunks()
{
    // Sécurité vitale : Ne génère les adresses OpenGL qu'une seule fois au
    // démarrage
    if (count == 0)
    {
        glGenVertexArrays(chunks, (GLuint *)&VAO);
        glGenBuffers(chunks, (GLuint *)&VBO);
        glGenBuffers(chunks, (GLuint *)&EBO);
    }

    int tempCount = 0;
    for (int x = -ChunkView; x < ChunkView; x += 1)
    {
        for (int z = -ChunkView; z < ChunkView; z += 1)
        {
            // On utilise bien la position relative à l'offset actuel !
            CreateChunk((x + offsetX) * ChunkSize, 0, (z + offsetZ) * ChunkSize,
                        tempCount, false);
            tempCount++;
        }
    }
    count = tempCount; // Confirme le nombre total de chunks (900)
}

void DrawChunk()
{
    Draw(count);
}

void Update()
{
    float radius = glm_vec3_norm(cameraPos);
    if (radius < 1.0f)
        radius = 1.0f;

    // 1. DÉTECTION DE LA FACE : Quelle face du cube est la plus proche de la
    // caméra ?
    int newFace = 0;
    float ax = fabsf(cameraPos[0]);
    float ay = fabsf(cameraPos[1]);
    float az = fabsf(cameraPos[2]);

    if (ay >= ax && ay >= az)
        newFace = cameraPos[1] > 0 ? 0 : 2; // TOP / BOTTOM
    else if (ax >= ay && ax >= az)
        newFace = cameraPos[0] > 0 ? 1 : 3; // RIGHT / LEFT
    else
        newFace = cameraPos[2] > 0 ? 4 : 5; // FRONT / BACK

    // 2. CONVERSION : On aplatit la sphère pour retrouver notre système
    // "Minecraft" 2D classique
    float u = 0.0f, v = 0.0f;
    if (newFace == 0)
    {
        u = cameraPos[0] / ay;
        v = cameraPos[2] / ay;
    }
    else if (newFace == 1)
    {
        u = -cameraPos[2] / ax;
        v = cameraPos[1] / ax;
    }
    else if (newFace == 2)
    {
        u = cameraPos[0] / ay;
        v = -cameraPos[2] / ay;
    }
    else if (newFace == 3)
    {
        u = cameraPos[2] / ax;
        v = cameraPos[1] / ax;
    }
    else if (newFace == 4)
    {
        u = cameraPos[0] / az;
        v = cameraPos[1] / az;
    }
    else if (newFace == 5)
    {
        u = -cameraPos[0] / az;
        v = cameraPos[1] / az;
    }

    float FACE_SIZE = 1200.0f;
    float virtualCamX = u * (FACE_SIZE / 2.0f);
    float virtualCamZ = v * (FACE_SIZE / 2.0f);

    int targetOffsetX = (int)roundf(virtualCamX / ChunkSize);
    int targetOffsetZ = (int)roundf(virtualCamZ / ChunkSize);

    // 3. TRANSITION DE FACE : Si vous passez la frontière, on recentre
    // immédiatement la grille entière
    if (newFace != currentFace)
    {
        currentFace = newFace;
        offsetX = targetOffsetX;
        offsetZ = targetOffsetZ;
        GenerateChunks(); // On réécrit massivement la carte graphique pour la
                          // nouvelle face !
        return; // On skip la mise à jour fine pour cette frame
    }

    // 4. MISE À JOUR FINE : Chargement classique des chunks quand vous marchez
    // sur une même face
    bool xUpdate = false;
    bool zUpdate = false;
    int addAxisX = 0;
    int addAxisZ = 0;
    int ChunkLength = ChunkView * 2;

    if (virtualCamX > (offsetX + 1) * ChunkSize)
    {
        offsetX++;
        addAxisX = -1;
        xUpdate = true;
    }
    else if (virtualCamX < (offsetX - 1) * ChunkSize)
    {
        offsetX--;
        addAxisX = 0;
        xUpdate = true;
    }

    if (virtualCamZ < (offsetZ - 1) * ChunkSize)
    {
        offsetZ--;
        zUpdate = true;
        addAxisZ = 0;
    }
    else if (virtualCamZ > (offsetZ + 1) * ChunkSize)
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
            CreateChunk((float)(sign * ChunkView + xOffset) * ChunkSize, 0,
                        (float)(i + offsetZ) * ChunkSize,
                        Modulo(Modulo(index + offsetZ, ChunkLength)
                                   + ChunkLength * xOffset,
                               count),
                        false);
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
            CreateChunk(
                (float)(i + offsetX) * ChunkSize, 0,
                (float)(sign * ChunkView + zOffset) * ChunkSize,
                ModuloZ(Modulo(index + offsetX, ChunkLength) * ChunkLength
                            + zOffset,
                        ChunkLength, Modulo(index + offsetX, ChunkLength)),
                false);
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
                CreateChunk((float)(x + offsetX) * ChunkSize, 0,
                            (float)(z + offsetZ) * ChunkSize, i, true);
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
    return r < 0 ? r + b : r;
}

int ModuloZ(int a, int b, int i)
{
    int r = a - i * b;
    r %= b;
    return r < 0 ? r + b + i * b : r + i * b;
}

void ClearChunk()
{
    glDeleteVertexArrays(count, (GLuint *)&VAO);

    glDeleteBuffers(count, (GLuint *)&VBO);
    glDeleteBuffers(count, (GLuint *)&EBO);
    // glDeleteBuffers(count, (GLuint *)&VAO);
}
