#ifndef FIBHEAP_H
#define FIBHEAP_H

#include "Element.h"

typedef struct fibheap FibHeap;

FibHeap *initFibHeap();
void insertFib(FibHeap *heap, Element data);
//merge other to heap and free other
void mergeFib(FibHeap *heap, FibHeap *other);
Element deleteMinFib(FibHeap *heap);
Element deleteFib(FibHeap *heap, int key);
void decreaseKeyFib(FibHeap *heap, int target, int newKey);

void printFib(FibHeap *heap);
void destroyFib(FibHeap *heap);

#define debug(message) printf("%s\n", message); fflush(stdout)


#endif