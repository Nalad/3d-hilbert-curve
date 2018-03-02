#include <stdio.h>
#include <stdlib.h>

#include "List.h"

struct Node {
    Item data;
    struct Node *next;
};

struct ListType {
    struct Node *top;
    int size;
};

static void terminate (const char *message);

List ListCreate(void) {
    List l = malloc(sizeof(*l));
    if (l == NULL) {
        terminate("Error in create: list could not be created");
    }
    
    l->top = NULL;
    l->size = 0;
    return l;
}

void ListDestroy(List l) {
    ListMakeEmpty(l);
    free(l);
}

void ListMakeEmpty(List l) {
    while (!ListIsEmpty(l)) {
        Vec3DDestroy(ListRemoveLastItem(l));
    }
}

bool ListIsEmpty(List l) {
    return l->top == NULL;
}

bool ListIsFull(List l) {
    return false;
}

void ListAdd(List l, Item i) {
    struct Node *newNode = malloc(sizeof(*newNode));
    if (newNode == NULL) {
        terminate("Error in push: stack is full.");
    }

    newNode->data = i;
    newNode->next = l->top;
    l->top = newNode;
    (l->size)++;
}

Item ListRemoveLastItem(List l) {
    if (ListIsEmpty(l)) {
        terminate("Error in ListRemoveLastItem: list is empty.");
    }

    struct Node *oldTop = l->top;
    Item i = oldTop->data;
    l->top = oldTop->next;
    free(oldTop);
    return i;
}

void ListPrintAllItems(List l) {
    struct Node *node = l->top;
    while (node != NULL) {
        Item i = node->data;
        Vec3DPrint(i);
        node = node->next;
    }
}

void ListAddAll(List l, List e) {
    struct Node *lastNode = l->top;
    if (lastNode == NULL) {
        l->top = e->top;
        l->size = e->size;
        free(e);
        return;
    }
    while (lastNode->next != NULL) {
        lastNode = lastNode->next;
    }
   
    lastNode->next = e->top;
    l->size += e->size;

    free(e);
}

int ListSize(List l) {
    return l->size;
}

Item ListGetItem(List l, int num) {
    struct Node *node = l->top;
    for (int i = 0; i < num; i++) {
        node = node->next;
    }

    return node->data;
}

static void terminate (const char *message) {
    printf("%s\n", message);
    exit(EXIT_FAILURE);
}
