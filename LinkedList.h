#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <stdbool.h>

#include "Element.h"

typedef struct linkedlist LinkedList;

LinkedList* initList();
void addlistHead(LinkedList *list, Element data);
void addlistTail(LinkedList *list, Element data);
Element deletelistTop(LinkedList *list);
Element getlistTop(LinkedList *list);

void printList(LinkedList *list);
void destroylist(LinkedList *list);

bool listEmpty(LinkedList *list);


#endif
