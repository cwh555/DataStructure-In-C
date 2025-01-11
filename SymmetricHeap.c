#include "SymmetricHeap.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct symmetric_heap{
    int size;
    int dataNum;
    Element *data;
};

//--helper
inline static int getlc(int index){
    return index * 2 + 1;
}

inline static int getrc(int index){
    return index * 2 + 2;
}

inline static int getparent(int index){
    return (index - 1) / 2;
}

inline static int getls(int index){
    return (index % 2 == 0 ? index - 1 : index);
}

inline static int getrs(int index){
    return (index % 2 == 1 ? index + 1 : index);
}

inline static int getsibling(int index){
    return (index % 2 == 0 ? index - 1 : index + 1);
}

//if key <= right sibling's key
bool Rule1(SymHeap *heap, int indexNow, Element *new){
    if(indexNow % 2 == 0)
        return new->key >= heap->data[indexNow - 1].key;
    else
        return new->key <= heap->data[indexNow + 1].key;
}

//if n has grand node x, x's lc <= n
bool Rule2(SymHeap *heap, int indexNow, Element *new){
    int grand = getparent(getparent(indexNow));
    return (getparent(indexNow) == 0 ||
            heap->data[getlc(grand)].key <= new->key);
}

//if n has grand node x, x's rc >= n
bool Rule3(SymHeap *heap, int indexNow, Element *new){
    int grand = getparent(getparent(indexNow));
    return (getparent(indexNow) == 0 ||
            heap->data[getrc(grand)].key >= new->key);

}
//--end

SymHeap *initSym(int size){
    SymHeap *heap = (SymHeap *)malloc(sizeof(SymHeap));
    heap->size = size;
    heap->dataNum = 0;
    heap->data = (Element *)malloc(sizeof(Element) * heap->size);
    return heap;
}

void insertSym(SymHeap *heap, Element newData){
    if(heap->dataNum == heap->size - 1){
        heap->size *= 2;
        heap->data = realloc(heap->data, sizeof(Element) * heap->size);
        assert(heap->data != NULL);
    }

    int indexNow = ++heap->dataNum;
    while(indexNow > 0){
        if(getsibling(indexNow) <= heap->dataNum && !Rule1(heap, indexNow, &newData)){
            heap->data[indexNow] = heap->data[getsibling(indexNow)];
            indexNow = getsibling(indexNow);
        }
        else if(!Rule2(heap, indexNow, &newData)){
            int grand = getparent(getparent(indexNow));
            heap->data[indexNow] = heap->data[getlc(grand)];
            indexNow = getlc(grand);
        }
        else if(!Rule3(heap, indexNow, &newData)){
            int grand = getparent(getparent(indexNow));
            heap->data[indexNow] = heap->data[getrc(grand)];
            indexNow = getrc(grand);
        }
        else
            break;
    }
    assert(indexNow > 0);
    heap->data[indexNow] = newData;

    return;
}

void mantain(SymHeap *heap, int indexNow, Element *data){
    bool violate = true;
    while(indexNow <= heap->dataNum && violate){
        int lc = getlc(indexNow);
        int rslc = getlc(getrs(indexNow));
        int rc = getrc(indexNow);
        int lsrc = getrc(getls(indexNow));
        violate = false;

        if(getsibling(indexNow) <= heap->dataNum && !Rule1(heap, indexNow, data)){
            Element temp = heap->data[getsibling(indexNow)];
            heap->data[getsibling(indexNow)] = *data;
            *data = temp;
        }
        
        //if in the left sibling
        if(!violate && indexNow % 2 == 1 && lc <= heap->dataNum){
            int changeIndex = lc;
            if(rslc <= heap->dataNum && heap->data[rslc].key < heap->data[lc].key)
                changeIndex = rslc;

            if(heap->data[changeIndex].key < data->key){
                heap->data[indexNow] = heap->data[changeIndex];
                indexNow = changeIndex;
                violate = true;
            }
        }

        //if in the right sibling
        if(!violate && indexNow % 2 == 0 && lsrc <= heap->dataNum){
            int changeIndex = lsrc;
            if(rc <= heap->dataNum && heap->data[rc].key > heap->data[lsrc].key)
                changeIndex = rc;

            if(heap->data[changeIndex].key > data->key){
                heap->data[indexNow] = heap->data[changeIndex];
                indexNow = changeIndex;
                violate = true;
            }
        }
    }
    heap->data[indexNow] = *data;
    return;
}

Element getSymMax(SymHeap *heap){
    if(heap->dataNum == 1)
        return heap->data[1];
    else if(heap->dataNum == 0){
        fprintf(stderr, "heap is empty\n");
        exit(-1);
    }

    Element result = heap->data[2];

    Element update = heap->data[heap->dataNum--];
    mantain(heap, 2, &update);
    return result;
}

Element getSymMin(SymHeap *heap){
    if(heap->dataNum == 0){
        fprintf(stderr, "heap is empty\n");
        exit(-1);
    }

    Element result = heap->data[1];

    Element update = heap->data[heap->dataNum--];
    mantain(heap, 1, &update);
    return result;
}

void destroySym(SymHeap *heap){
    free(heap->data);
    free(heap);
}

void printSym(SymHeap *heap){
    printf("heap\n");
    for(int i = 1; i <= heap->dataNum; i++)
        printf("%d ", heap->data[i].key);
    printf("\n");
    return;
}
