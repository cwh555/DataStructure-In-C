#include "IntervalHeap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define SWAP(a, b, temp) do{\
            temp = a; a = b; b = temp;\
        }while(0)

typedef struct node{
    Element lower;
    Element upper;
}Node;

struct intervalHeap{
    int size;
    int dataNum;
    Node *data;
};

//--helper
inline static int getparent(int index){
    return (index - 1) / 2;
}

inline static int getlc(int index){
    return index * 2 + 1;
}

inline static int getrc(int index){
    return index * 2 + 2;
}

inline static int maxIndex(int num){
    return (num - 1) / 2;
}

//-end

IntervalHeap *initInter(int size){
    IntervalHeap *heap = (IntervalHeap *)malloc(sizeof(IntervalHeap));
    heap->size = size;
    heap->dataNum = 0;
    heap->data = (Node *)malloc(sizeof(Node) * heap->size);
    assert(heap->data != NULL);
    return heap;
}

void updateLower(IntervalHeap *heap, Element *data, int indexNow){
    int parent = getparent(indexNow);
    while(indexNow > 0 && heap->data[parent].lower.key > data->key){
        heap->data[indexNow].lower = heap->data[parent].lower;
        indexNow = parent;
        parent = getparent(indexNow);
    }
    heap->data[indexNow].lower = *data;
    return;
}

void updateUpper(IntervalHeap *heap, Element *data, int indexNow){
    int parent = getparent(indexNow);
    while(indexNow > 0 && heap->data[parent].upper.key < data->key){
        heap->data[indexNow].upper = heap->data[parent].upper;
        indexNow = parent;
        parent = getparent(indexNow);
    }
    heap->data[indexNow].upper = *data;
    return;
}

void insertInter(IntervalHeap *heap, Element data){
    if(maxIndex(heap->dataNum) == heap->size){
        heap->size *= 2;
        heap->data = realloc(heap->data, sizeof(Node) * heap->size);
        assert(heap->data != NULL);
    }

    int indexNow = maxIndex(++heap->dataNum);
    if(indexNow == 0 || 
        (heap->data[getparent(indexNow)].lower.key <= data.key && 
        heap->data[getparent(indexNow)].upper.key >= data.key)){
        if(heap->dataNum % 2 == 0){
            if(heap->data[indexNow].lower.key > data.key){
                Element temp;
                SWAP(data, heap->data[indexNow].lower, temp);
            }
            heap->data[indexNow].upper = data;
        }
        else
            heap->data[indexNow].lower = data;
    }
    else if(heap->data[getparent(indexNow)].lower.key > data.key){
        heap->data[indexNow].upper = heap->data[indexNow].lower;
        heap->data[indexNow].lower = heap->data[getparent(indexNow)].lower;
        updateLower(heap, &data, getparent(indexNow));
    }
    else{
        if(heap->dataNum % 2 == 1)
            heap->data[indexNow].lower = heap->data[getparent(indexNow)].upper;
        else
            heap->data[indexNow].upper = heap->data[getparent(indexNow)].upper;
        updateUpper(heap, &data, getparent(indexNow));
    }
    return;
}

Element getInterMin(IntervalHeap *heap){
    if(heap->dataNum == 0){
        fprintf(stderr, "the heap is empty\n");
        exit(-1);
    }

    Element result = heap->data[0].lower;

    Element update = heap->data[maxIndex(heap->dataNum)].lower;
    heap->data[maxIndex(heap->dataNum)].lower = heap->data[maxIndex(heap->dataNum)].upper;
    heap->dataNum--;

    if(heap->dataNum == 1){
        heap->data[0].lower = heap->data[0].upper;
        return result;
    }

    int indexNow = 0;
    int indexMax = maxIndex(heap->dataNum);
    bool violate = true;
    while(getlc(indexNow) <= indexMax && violate){
        int changeIndex = getlc(indexNow);

        if((getrc(indexNow) <= indexMax) && 
            heap->data[getrc(indexNow)].lower.key < heap->data[getlc(indexNow)].lower.key)
            changeIndex = getrc(indexNow);
        
        //interval update
        if(heap->data[indexNow].upper.key < update.key){
            Element temp;
            SWAP(heap->data[indexNow].upper, update, temp);
        }


        violate = false;
        if(heap->data[changeIndex].lower.key < update.key){
            heap->data[indexNow].lower = heap->data[changeIndex].lower;
            indexNow = changeIndex;
            violate = true;
        }
    }
    if(indexNow * 2 + 2 <= heap->dataNum && heap->data[indexNow].upper.key < update.key){
        heap->data[indexNow].lower = heap->data[indexNow].upper;
        heap->data[indexNow].upper = update;
    }
    else
        heap->data[indexNow].lower = update;
        
    return result;
}

Element getInterMax(IntervalHeap *heap){
    if(heap->dataNum == 0){
        fprintf(stderr, "the heap is empty\n");
        exit(-1);
    }
    else if(heap->dataNum == 1){
        heap->dataNum--;
        return heap->data[0].lower;
    }

    Element result = heap->data[0].upper;
    Element update;
    if(heap->dataNum % 2 == 0)
        update = heap->data[maxIndex(heap->dataNum)].upper;
    else{
        update = heap->data[maxIndex(heap->dataNum)].lower;
        //in order to update conveniently
        heap->data[maxIndex(heap->dataNum)].upper = update;
    }
    heap->dataNum--;    
    
    if(heap->dataNum == 1)
        return result;

    int indexNow = 0;
    int indexMax = maxIndex(heap->dataNum);
    bool violate = true;
    while(getlc(indexNow) <= indexMax && violate){
        int changeIndex = getlc(indexNow);

        if(getrc(indexNow) <= indexMax &&
            (heap->data[getrc(indexNow)].upper.key > heap->data[changeIndex].upper.key))
            changeIndex = getrc(indexNow);

        //interval update
        if(heap->data[indexNow].lower.key > update.key){
            Element temp;
            SWAP(update, heap->data[indexNow].lower, temp);
        }

        violate = false;
        if(heap->data[changeIndex].upper.key > update.key){
            heap->data[indexNow].upper = heap->data[changeIndex].upper;
            indexNow = changeIndex;
            violate = true;
        }
    }
    if(indexNow == indexMax && heap->dataNum % 2 == 1)
        heap->data[indexNow].lower = update;
    else if(heap->data[indexNow].lower.key > update.key){
        heap->data[indexNow].upper = heap->data[indexNow].lower;
        heap->data[indexNow].lower = update;
    }
    else
        heap->data[indexNow].upper = update;
    return result;
}

void printInter(IntervalHeap *heap){
    if(heap->dataNum == 0){
        printf("empty\n");
        return;
    }
    
    printf("heap\n");
    int indexMax = maxIndex(heap->dataNum);
    for(int i = 0; i < indexMax; i++){
        assert(heap->data[i].lower.key <= heap->data[i].upper.key);
        printf("%d to %d\n", heap->data[i].lower.key, heap->data[i].upper.key);
    }


    if(heap->dataNum % 2 == 1)
        printf("%d\n", heap->data[indexMax].lower.key);
    else
        printf("%d to %d\n", heap->data[indexMax].lower.key, heap->data[indexMax].upper.key);
    return;
}

void destroyInter(IntervalHeap *heap){
    free(heap->data);
    free(heap);
    return;
}