#include <cglm/cglm.h>
#include <stdlib.h>
#include <stdio.h>

#include "Destroy.h"

struct DestroyList;
#include <Generators/Chunk/Region.h>
#include <Physics/Ray.h>
#include <Utils/FileUtils.h>
#include <Generators/Chunk/ChunkManager.h>
#include <Player/Camera.h>
#include <Player/Controller.h>

int size = 0;

void Add(struct DestroyList* list, int element)
{
    struct DestroyList* temp, * ptr;
    temp = malloc(sizeof(struct DestroyList));

    temp->data = element;
    temp->next = NULL;

    if (list == NULL)
        list = temp;
    else
    {
        ptr = list;
        while (ptr->next != NULL)
            ptr = ptr->next;
        ptr->next = temp;
    } 
}

int GetList(struct DestroyList* list, int n)
{
    while (n > 0 && list->next != NULL)
    {
        list = list->next;

        n--;
    }
    return list->data;
}

void DestroyBlock()
{ 
    float destroyed[3];
    if (!RayCast(cameraPos, forward, 10, 0.1, destroyed))
        return;
    PrintArr3(destroyed);
    UpdateDestroyed(AddBlockState(destroyed));
}
