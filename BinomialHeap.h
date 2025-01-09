/*
support insert, deleteMin, merge
*/

#ifndef BINOHEAP_H
#define BINOHEAP_H

#include "Element.h"

typedef struct binheap BinHeap;

BinHeap *initBinomial(void);
//after merge other will be free
void mergeBinomial(BinHeap *heap, BinHeap *other);
void insertBinomial(BinHeap *heap, Element data);
Element getBinomialMin(BinHeap *heap);

void printBinomial(BinHeap *heap);
void destroyBinomial(BinHeap *heap);

#endif