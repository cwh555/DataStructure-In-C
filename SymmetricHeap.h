/*
double ended priority queue
*/

#ifndef SYMMHEAP_H
#define SYMMHEAP_H
#include "Element.h"

typedef struct symmetric_heap SymHeap;

SymHeap *initSym(int size);
void insertSym(SymHeap *heap, Element data );
Element getSymMax(SymHeap *heap);
Element getSymMin(SymHeap *heap);

void destroySym(SymHeap *heap);
void printSym(SymHeap *heap);

#endif