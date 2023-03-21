#ifndef CHUNK_MANAGER
#define CHUNK_MANAGER

#define ChunkView 20

int Modulo(int a, int b);

void GenerateChunks();

void DrawChunk();

void Update();

void ClearChunk();

void UpdateDestroyed(int chunkIndex);

#endif // !CHUNK_MANAGER