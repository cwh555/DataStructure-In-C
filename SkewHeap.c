#include "SkewHeap.h"
#include <stdlib.h>
#include <stdio.h>

#define SWAP(a, b, temp) do{\
            temp = a; a = b; b = temp;\
        }while(0)


typedef struct node{
    Element data;
    struct node *lc, *rc;
}Node;

struct skewheap{
    Node *root;
};

SkewHeap *initSkew(void){
    SkewHeap *heap = (SkewHeap *)malloc(sizeof(SkewHeap));
    heap->root = NULL;
    return heap;
}

Node *skewUnion(Node *a, Node *b){
    Node *temp;
    
    if(a->data.key > b->data.key)
        SWAP(a, b, temp);

    //union
    if(a->rc == NULL)
        a->rc = b;
    else
        a->rc = skewUnion(a->rc, b);

    //maintain(swap lc, rc)
    SWAP(a->rc, a->lc, temp);

    return a;
}


//after merge other will be free
void mergeSkew(SkewHeap *heap, SkewHeap *other){
    if(heap->root == NULL)
        heap->root = other->root;
    else if(other->root != NULL)
        heap->root = skewUnion(heap->root, other->root);
    
    free(other);
    return;
}

Node *newNode(Element data){
    Node *new = (Node *)malloc(sizeof(Node));
    new->data = data;
    new->lc = new->rc = NULL;
    return new;
}

void insertSkew(SkewHeap *heap, Element data){
    SkewHeap *temp = initSkew();
    temp->root = newNode(data);

    mergeSkew(heap, temp);
    return;
}

Element getSkewMin(SkewHeap *heap){
    Element result = heap->root->data;
    Node *old = heap->root;

    SkewHeap *temp1 = initSkew();
    temp1->root = old->lc;

    SkewHeap *temp2 = initSkew();
    temp2->root = old->rc;

    mergeSkew(temp1, temp2);
    heap->root = temp1->root;

    free(old);
    return result;
}

void printHelper(Node *a){
    if(a == NULL)
        return;
    
    printHelper(a->lc);
    printf("%d ", a->data.key);
    printHelper(a->rc);
    return;
}

void printSkew(SkewHeap *heap){
    if(heap == NULL){
        printf("empty\n");
        return;    
    }

    printHelper(heap->root);
    printf("\n");
    return;
}

void destroyHelper(Node *a){
    if(a == NULL)
        return;
    
    destroyHelper(a->lc);
    destroyHelper(a->rc);
    free(a);
    return;
}

void destroySkew(SkewHeap *heap){
    if(heap == NULL)
        return;

    destroyHelper(heap->root);
    free(heap);
}