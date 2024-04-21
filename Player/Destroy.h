#ifndef DESTROY
#define DESTROY

typedef struct DestroyList
{
    int data;
    struct DestroyList *next;
} DestroyList;

/*
   struct chunkList
   {
   int index;
   struct DestroyList* destroyList;
   struct chunkList* nextChunk;
};*/

extern struct DestroyList *list;

extern int size;

void Add(struct DestroyList *list, int element);

void DestroyBlock();

int GetList(struct DestroyList *list, int n);

#endif // !DESTROY
