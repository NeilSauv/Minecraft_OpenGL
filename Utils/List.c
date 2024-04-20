#include <stdlib.h>
#include <stdio.h>

#include "../Utils/Headers/UtilsHeaders.h"

struct IntList
{
	int *data;
	int size;
};

void InitList(struct IntList* list)
{
	list->size = 0;
}

void AddList(struct IntList *list, int element)
{
	list->data = calloc(1, sizeof(int));
	if (list->data == NULL)
	{
		printf("Allocation Error List");
		exit(1);
	}
	list->data[0] = element;
	list->size++;
}

unsigned int GetArray(struct IntList* list)
{
	return list->data[0];
}

void ClearList(struct IntList* list)
{
	free(list->data);
}