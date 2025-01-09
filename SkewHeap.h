/*
another version of leftist tree;
support insert getMin, merge
*/

#ifndef SKEWHEAP_H
#define SKEWHEAP_H
#include "Element.h"

typedef struct skewheap SkewHeap;

SkewHeap *initSkew(void);
void insertSkew(SkewHeap *heap, Element data);
Element getSkewMin(SkewHeap *heap);

//after merge other will be free
void mergeSkew(SkewHeap *heap, SkewHeap *other);
void printSkew(SkewHeap *heap);

void destroySkew(SkewHeap *heap);

#endif