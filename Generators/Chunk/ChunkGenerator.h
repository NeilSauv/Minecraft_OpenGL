#ifndef CHUNK_GENERATOR
#define CHUNK_GENERATOR

#include <stdbool.h>


#define ChunkSize 16
#define ChunkHeight 256
#define ChunkView 40

extern unsigned int indices[];
extern float texCoords[];

extern float translations[ChunkSize * ChunkSize * ChunkHeight][3];
extern int rendering[ChunkSize * ChunkSize * ChunkHeight*4];

extern unsigned int VBO[ChunkView * ChunkView * 4];
extern unsigned int VAO[ChunkView * ChunkView * 4];
extern unsigned int EBO[ChunkView * ChunkView * 4];
extern unsigned int faceVBO[ChunkView * ChunkView * 4];
extern unsigned int instanceVBO[ChunkView * ChunkView * 4];
extern unsigned int renderVBO[ChunkView * ChunkView * 4];

void CreateChunk(int xAxis,int yAxis, int zAxis, int divisorCount, bool destroyBlock);

void Draw(int i);

#endif
