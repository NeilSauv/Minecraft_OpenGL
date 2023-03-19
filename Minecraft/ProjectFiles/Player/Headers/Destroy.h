#ifndef DESTROY
#define DESTROY

#include "../../Utils/Headers/List.h"

struct destroyList
{
	int data;
	struct destroyList* next;
};
/*
struct chunkList
{
	int index;
	struct destroyList* destroyList;
	struct chunkList* nextChunk;
};*/

extern struct destroyList* list;

extern int size;

void Add(struct destroyList* list, int element);

void DestroyBlock();

int GetList(struct destroyList* list, int n);

#endif // !DESTROY
