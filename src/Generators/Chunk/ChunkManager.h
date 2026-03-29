#ifndef CHUNK_MANAGER
#define CHUNK_MANAGER

extern int currentFace; // 0=Top, 1=Right, 2=Bottom, 3=Left, 4=Front, 5=Back

int Modulo(int a, int b);
void GenerateChunks();
void DrawChunk();
void Update();
void ClearChunk();
void UpdateDestroyed(int chunkIndex);

#endif // !CHUNK_MANAGER