#ifndef CHUNK_GENERATOR
#define CHUNK_GENERATOR

#include <stdbool.h>

#define ChunkSize 16
#define ChunkHeight 256
#define ChunkView 15 * 4

#define VERTICES_PER_CHUNK ((ChunkSize + 1) * (ChunkSize + 1))

// Pour relier ces sommets, on fait des carrés de 2 triangles (6 indices) par
// case (ChunkSize * ChunkSize)
#define INDICES_PER_CHUNK (ChunkSize * ChunkSize * 6)

// Nouvelles structures de données (au revoir les VBO d'instances)
extern float chunkVertices[VERTICES_PER_CHUNK * 6];
extern unsigned int chunkIndices[INDICES_PER_CHUNK];

extern unsigned int VBO[ChunkView * ChunkView * 4];
extern unsigned int VAO[ChunkView * ChunkView * 4];
extern unsigned int EBO[ChunkView * ChunkView * 4];

void CreateChunk(int xAxis, int yAxis, int zAxis, int divisorCount,
                 bool destroyBlock);

void Draw(int i);

#endif
