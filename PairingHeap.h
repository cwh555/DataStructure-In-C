/*
two type for merging TWOPASS and MULTIPASS
*/

#ifndef PAIRINGHEAP_H
#define PAIRINGHEAP_H
#include "Element.h"

typedef struct pairing PairHeap;

typedef enum {
    TWOPASS, MULTIPASS
}PAIRTYPE;

PairHeap *initPair(PAIRTYPE type);
//merge to heap and free other
void mergePair(PairHeap *heap, PairHeap *other);
void insertPair(PairHeap *heap, Element data);
Element deleteMinPair(PairHeap *heap);
Element deletePair(PairHeap *heap, int key);
void decreaseKeyPair(PairHeap *heap, int target, int newKey);

void printPair(PairHeap *heap);
void destroyPair(PairHeap *heap);

#endif