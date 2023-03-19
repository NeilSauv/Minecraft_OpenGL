#include <cglm/cglm.h>
#include <stdlib.h>
#include <stdio.h>

#include "../Physics/Headers/Ray.h"
#include "../Player/Headers/Camera.h"
#include "../Player/Headers/Controller.h"
#include "../Utils/Headers/FileUtils.h"
#include "../Generators/Chunk/Headers/ChunkManager.h"
#include "../Generators/Chunk/Headers/ChunkGenerator.h"
#include "../Generators/Chunk/Headers/Region.h"

struct destroyList 
{
	int data;
	struct destroyList* next;
};

int size = 0;

void Add(struct destroyList* list, int element);

struct destroyList* list;

void Add(struct destroyList* list, int element)
{
	struct destroyList* temp, * ptr;
	temp = (struct destroyList*)malloc(sizeof(struct destroyList));

	temp->data = element;
	temp->next = NULL;  
	
	if (list == NULL)
	{
		list = temp;
	}
	else
	{
		ptr = list;
		while (ptr->next != NULL)
			ptr = ptr->next;
		ptr->next = temp;
	} 
}

int GetList(struct destroyList* list, int n)
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