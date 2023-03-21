#ifndef DESTROY
#define DESTROY

#include "../../Utils/Headers/UtilsHeaders.h"

struct DestroyList
{
	int data;
	struct DestroyList* next;
};

typedef struct DestroyList DestroyList;
/*
struct chunkList
{
	int index;
	struct DestroyList* destroyList;
	struct chunkList* nextChunk;
};*/

extern struct DestroyList* list;

extern int size;

void Add(struct DestroyList* list, int element);

void DestroyBlock();

int GetList(struct DestroyList* list, int n);

#endif // !DESTROY
