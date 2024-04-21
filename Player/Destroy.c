#include "Destroy.h"

#include <cglm/cglm.h>
#include <stdio.h>
#include <stdlib.h>

struct DestroyList;
#include <Generators/Chunk/ChunkManager.h>
#include <Generators/Chunk/Region.h>
#include <Physics/Ray.h>
#include <Player/Camera.h>
#include <Player/Controller.h>
#include <Utils/FileUtils.h>

int size = 0;

void Add(struct DestroyList *list, int element)
{
    struct DestroyList *temp, *ptr;
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

int GetList(struct DestroyList *list, int n)
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
