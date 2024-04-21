#ifndef LIST
#define LIST

struct IntList;

void InitList(struct IntList *list);

void AddList(struct IntList *list, int element);

unsigned int GetArray(struct IntList *list);

void ClearList(struct IntList *list);

#endif // !LIST
