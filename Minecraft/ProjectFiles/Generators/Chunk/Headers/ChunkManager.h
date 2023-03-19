#ifndef CHUNK_MANAGER
#define CHUNK_MANAGER

#define ChunkView 20

extern struct osn_context* ctx;

int Modulo(int a, int b);

void GenerateChunks();

void DrawChunk();

void Update();

void UpdateDestroyed();

void ClearChunk();

#endif // !CHUNK_MANAGER