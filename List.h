#ifndef LIST
#define LIST

#include <stdbool.h>
#include "Vector3D.h"

typedef Vector3D Item;

typedef struct ListType *List;

List ListCreate(void);
void ListDestroy(List l);
void ListMakeEmpty(List l);
bool ListIsEmpty(List l);
bool ListIsFull(List l);
void ListAdd(List l, Item i);
Item ListRemoveLastItem(List l);
void ListPrintAllItems(List l);
void ListAddAll(List l, List e);
Item ListGetItem(List l, int i);
int ListSize(List l);

#endif
