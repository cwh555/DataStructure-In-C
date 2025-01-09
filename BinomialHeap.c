#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "BinomialHeap.h"

#define SWAP(a, b, temp) do{\
            temp = a; a = b; b = temp;\
        }while(0)


typedef struct node{
    Element data;
    struct node *sibling, *child;
    int degree;
}Node;

struct binheap{
    Node *head; //head has no data, put the first node at head->sibling
    Node *min;
};

Node *newNode(Element data){
    Node *new = (Node *)malloc(sizeof(Node));
    assert(new != NULL);

    new->child = new->sibling = NULL;
    new->degree = 0;
    new->data = data;
    return new;
}

BinHeap *initBinomial(void){
    BinHeap *heap = (BinHeap *)malloc(sizeof(BinHeap));
    assert(heap != NULL);

    Element data = {.key = -1};
    heap->head = newNode(data);
    heap->min = NULL;
    return heap;
}

Node *mergeTree(Node *a, Node *b){
    assert(a->degree == b->degree);
    Node *buffer;

    if(a->data.key > b->data.key)
        SWAP(a, b, buffer);

    //link b to the sibling of a's child
    Node *now = a->child;
    if(now == NULL){
        assert(a->degree == 0);
        a->child = b;
    }
    else{
        while(now->sibling != NULL)
            now = now->sibling;
        now->sibling = b;
    }

    a->degree++;
    return a;
}

//--helper
void insertTree(BinHeap *heap, Node *new){
    if(heap->min == NULL || heap->min->data.key > new->data.key)
        heap->min = new;

    Node *before = heap->head;
    Node *now = before->sibling;

    while(now != NULL && now->degree <= new->degree){
        if(now->degree == new->degree){
            //merge two tree
            //break the link to avoid problem
            before->sibling = now->sibling;
            now->sibling = NULL;

            new = mergeTree(new, now);

            now = before->sibling;
        }
        else{
            before = now;
            now = now->sibling;
        }
    }

    //insert between before and now
    before->sibling = new;
    new->sibling = now;
    return;
}
//--end

//after merge other will be free
void mergeBinomial(BinHeap *heap, BinHeap *other){
    Node *waiting = other->head->sibling;
    while(waiting != NULL){
        Node *next = waiting->sibling;
        waiting->sibling = NULL;
        insertTree(heap, waiting);
        waiting = next;
    }

    free(other->head);
    free(other);

    return;    
}

void insertBinomial(BinHeap *heap, Element data){
    Node *new = newNode(data);

    insertTree(heap, new);
    return;
}

Element getBinomialMin(BinHeap *heap){
    if(heap->min == NULL){
        assert(heap->head->sibling == NULL);
        fprintf(stderr, "binomial heap is empty\n");
        exit(-1);
    }

    Element result = heap->min->data;

    Node *target = heap->min;
    Node *now = heap->head;
    while(now->sibling != target){
        assert(now != NULL);
        now = now->sibling;
    }
    assert(now->sibling == target);

    //let the subtree of target become another binheap
    BinHeap *temp = initBinomial();
    temp->head->sibling = target->child;

    //delete target
    now->sibling = target->sibling;
    target->sibling = target->child = NULL;

    //min continue to point to target 
    //in order to avoid min change without expect during merging

    //update heap by merge sub-binheap into it
    mergeBinomial(heap, temp);

    //update min if min == NULL then the heap must be empty
    now = heap->head->sibling;
    heap->min = now;
    while(now != NULL){
        if(now->data.key < heap->min->data.key)
            heap->min = now;
        now = now->sibling;
    }

    free(target);
    return result;
}

void printTree(Node *a){
    if(a->degree == 0)
        printf("T0 %d\n", a->data.key);
    else{
        printf("tree root %d  T%d start\n", a->data.key, a->degree);
        Node *temp = a->child;
        while(temp != NULL){
            printTree(temp);
            temp = temp->sibling;
        }
        printf("tree root %d T%d end\n", a->data.key, a->degree);
    }

    return;
}

void printBinomial(BinHeap *heap){
    if(heap->min == NULL){
        printf("empty\n");
        return;
    }
    printf("min : %d\n", heap->min->data.key);

    Node *temp = heap->head->sibling;
    while(temp != NULL){
        printf("top list\n");
        printTree(temp);

        temp = temp->sibling;
    }
    return;
}

void destroyHelper(Node *a){
    if(a == NULL)
        return;
    
    destroyHelper(a->child);
    destroyHelper(a->sibling);
    free(a);
    return;
}

void destroyBinomial(BinHeap *heap){
    destroyHelper(heap->head);
    free(heap);
    return;
}