#include "BinaryHeap.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define INITSIZE 100



//--dataStruct
struct binaryheap{
    int maxsize;
    int dataNum;
    Element **data;
    int (*compare)(Element *, Element *);
    int limit;
};


//--end dataStruct


//--helper
int maxcompare(Element *data1, Element *data2){
    return data1->key > data2->key;
}

int mincompare(Element *data1, Element *data2){
    return data1->key < data2->key;
}

static inline int getParent(int index){
    return (index - 1) / 2;
}

static inline int getLchild(int index){
    return 2 * index + 1;
}

static inline int getRchild(int index){
    return 2 * index + 2;
}
//--end helper


//--function
BinaryHeap* initBinaryHeap(TYPE type){
    BinaryHeap *heap = (BinaryHeap *)malloc(sizeof(BinaryHeap));
    heap->maxsize = INITSIZE;
    heap->dataNum = 0;
    heap->data = (Element **)malloc(sizeof(Element *) * heap->maxsize);
    
    if(type == MAXHEAP){
        heap->compare = maxcompare;
        heap->limit = INT32_MAX;
    }
    else{
        heap->compare = mincompare;
        heap->limit = INT32_MIN;
    }

    return heap;
}

void add(BinaryHeap *heap, Element newData){
    //expansion
    if(heap->dataNum == heap->maxsize){
        heap->maxsize *= 2;
        heap->data = (Element **)realloc(heap->data, sizeof(Element *) * heap->maxsize);

        if(!heap->data){
            fprintf(stderr, "realloc failure\n");
            exit(-1);
        }
    }


    int nowIndex = heap->dataNum;
    int parentIndex = getParent(nowIndex);
    while(nowIndex != 0 && heap->compare(&newData, heap->data[parentIndex])){
        heap->data[nowIndex] = heap->data[parentIndex];
        nowIndex = parentIndex;
        parentIndex = getParent(nowIndex);
    }

    Element *new = (Element *)malloc(sizeof(Element));
    *new = newData;
    heap->data[nowIndex] = new;
    heap->dataNum++;
    return;
}

Element getTop(BinaryHeap *heap){
    if(heap->dataNum == 0){
        Element empty = {.key = heap->limit};
        return empty;
    }

    Element top = *(heap->data[0]);
    free(heap->data[0]);

    heap->dataNum--;
    Element *updating = heap->data[heap->dataNum];
    int nowIndex = 0;
    int lcIndex = getLchild(nowIndex), rcIndex = getRchild(nowIndex);
    
    while(lcIndex < heap->dataNum){
        int minchild = lcIndex;
        if(rcIndex < heap->dataNum && heap->compare(heap->data[rcIndex], heap->data[lcIndex]))
            minchild = rcIndex;

        if(heap->compare(heap->data[minchild], updating))
            break;
        else{
            heap->data[nowIndex] = heap->data[minchild];
            nowIndex = minchild;
            lcIndex = getLchild(nowIndex), rcIndex = getRchild(nowIndex);
        }
    }

    heap->data[nowIndex] = updating;

    return top;
}

void destroy(BinaryHeap *heap){
    for(int i = 0; i < heap->dataNum; i++)
        free(heap->data[i]);
    free(heap->data);
    free(heap);
    return;
}

//--function end