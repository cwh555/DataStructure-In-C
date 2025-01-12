/*
double ended priority heap
*/

#ifndef INTERVALHEAP_H
#define INTERVALHEAP_H

#include "Element.h"

typedef struct intervalHeap IntervalHeap;

IntervalHeap *initInter(int size);
void insertInter(IntervalHeap *heap, Element data);
Element getInterMin(IntervalHeap *heap);
Element getInterMax(IntervalHeap *heap);

void printInter(IntervalHeap *heap);
void destroyInter(IntervalHeap *heap);

#endif