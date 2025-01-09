/*
init with MAXHEAP 0 or MINHEAP 1
*/



#ifndef BINARYHEAP_H
#define BINARYHEAP_H
#include "Element.h"

typedef enum type{
    MAXHEAP, MINHEAP
}TYPE;

typedef struct binaryheap BinaryHeap;

BinaryHeap* initBinaryHeap(TYPE type);
void add(BinaryHeap *heap, Element newData);
Element getTop(BinaryHeap *heap);

void destroy(BinaryHeap *heap);

#endif