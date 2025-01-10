#include "FibonacciHeap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SWAP(a, b, temp) do{\
            temp = a; a = b; b = temp;\
        }while(0)

typedef struct node{
    int degree;
    Element data;
    struct node *ls, *rs;   //sibling double linked list
    struct node *parent, *child;
    bool childcut;
}Node;

struct fibheap{
    int num;
    Node *min;
};

//--declaration
Node *newNode(Element data);
int getMaxDegree(int num);
Node *insertNodeToList(Node *a, Node *b);
Node *mergeNode(Node *a, Node *b);
void HeapintoRecord(Node **record, FibHeap *heap);
void printHelper(Node *a, int depth);
Node *findNode(Node *a, int key);
//--end

Node *newNode(Element data){
    Node *new = (Node *)malloc(sizeof(Node));
    new->child = new->parent = NULL;
    new->ls = new->rs = new;
    new->data = data;
    new->degree = 0;
    new->childcut = false;    
    return new;
}

int getMaxDegree(int num){
    if(num <= 1)
        return 0;
    
    return (int)floor(1.44 * log((double)num) / log(2) + 1.672);
}


FibHeap *initFibHeap(){
    FibHeap *heap = (FibHeap *)malloc(sizeof(FibHeap));
    heap->min = NULL;
    heap->num = 0;
    return heap;
}

//b to the a's sibling
Node *insertNodeToList(Node *a, Node *b){
    if(a == NULL)
        return b;

    assert(a->rs != NULL && b != NULL);
    b->rs = a->rs;//
    b->rs->ls = b;
    a->rs = b;
    b->ls = a;
    return a;
}

void insertFib(FibHeap *heap, Element data){
    Node *new = newNode(data);

    heap->min = insertNodeToList(heap->min, new);

    //update min
    if(heap->min->data.key > new->data.key)
        heap->min = new;

    heap->num++;
    return;   
}

Node *mergeNode(Node *a, Node *b){
    assert(a->degree == b->degree);

    Node *buffer;
    if(a->data.key > b->data.key)
        SWAP(a, b, buffer);

    //link b to the child of a
    a->child = insertNodeToList(a->child, b);
    b->parent = a;
    b->childcut = false;
    a->degree++;
    return a;
}

void HeapintoRecord(Node **record, FibHeap *heap){
    Node *start = heap->min;
    Node *now = start;
    if(now == NULL)
        return;
    
    //put all root into array
    //if the array already have one then merge two
    do{
        Node *next = now->rs;
        //break link
        now->ls = now->rs = now;
        //put into record
        while(record[now->degree] != NULL){
            Node *another = record[now->degree];
            record[now->degree] = NULL;

            //merge
            now = mergeNode(now, another);
        }
        record[now->degree] = now;

        now = next;
    }while(now != start);
    return;
}

//merge other to heap, free other and mantain min in the long run
void mergeFib(FibHeap *heap, FibHeap *other){
    heap->num += other->num;
    int maxDegree = getMaxDegree(heap->num);
    Node **record = (Node **)malloc(sizeof(Node *) * (maxDegree + 2));
    for(int i = 0; i <= maxDegree + 1; i++)
        record[i] = NULL;

    HeapintoRecord(record, heap);
    HeapintoRecord(record, other);

    //rebuild the list
    heap->min = NULL;
    for(int i = 0; i < maxDegree + 2; i++)
        if(record[i] != NULL){
            //printf("merge i %d\n", i);
            heap->min = insertNodeToList(heap->min, record[i]);
            //update min
            if(record[i]->data.key < heap->min->data.key)
                heap->min = record[i];
        }
    
    free(other);
    return;
}

Node *deleteListNode(Node *a){
    if(a->rs != a){
        a->rs->ls = a->ls;
        a->ls->rs = a->rs;
        Node *temp = a->rs;

        a->rs = a->ls = NULL;
        return temp;   
    }
    else{
        a->rs = a->ls = NULL;
        return NULL;
    }
}

Element deleteMinFib(FibHeap *heap){
    if(heap->min == NULL){
        fprintf(stderr, "heap is empty\n");
        exit(-1);
    }

    Node *target = heap->min;
    Element result = target->data;
    //put child to another heap and merge
    FibHeap *other = initFibHeap();
    other->min = target->child;

    if(other->min != NULL){
        //break parent
        Node *now = other->min;
        Node *start = now;
        do{
            now->parent = NULL;
            now->childcut = false;
            now = now->rs;
        }while(now != start);
    }

    //free min
    assert(target->ls != NULL && target->rs != NULL);
    heap->min = deleteListNode(target);
    
    target->rs = target->ls = NULL;
    target->child = target->parent = NULL;
    free(target);

    mergeFib(heap, other);
    return result;
}

Node *findNode(Node *a, int key){
    if(a == NULL)
        return NULL;

    Node *start = a;
    do{
        if(a->data.key == key)
            return a;
        Node *result = findNode(a->child, key);
        if(result != NULL)
            return result;

        a = a->rs;
    }while(a != start);
    return NULL;
}

void cascading(Node *target, FibHeap *heap){
    assert(target->childcut);
    Node *next = target->parent;
    assert(next != NULL);

    target->childcut = false;
    target->parent = NULL;
    next->child = deleteListNode(target);
    next->degree--;

    insertNodeToList(heap->min, target);

    //keep going
    if(next->childcut && next->parent != NULL)
        cascading(next, heap);
    else
        next->childcut = true;
    return;
}

Element deleteFib(FibHeap *heap, int key){
    Node *target = findNode(heap->min, key);
    if(target == NULL){
        fprintf(stderr, "the key is not in the heap\n");
        exit(-1);
    }
    else if(target == heap->min)
        return deleteMinFib(heap);

    Element result = target->data;

    FibHeap *other = initFibHeap();
    other->min = target->child;
    Node *now = other->min;
    if(other->min != NULL)
        do{
            now->parent = NULL;
            now = now->rs;            
        }while(now != other->min);

    //break link
    Node *parent = target->parent;
    if(parent != NULL){
        parent->child = deleteListNode(target);
        target->child = target->parent = NULL;
        free(target);

        //cascading
        parent->childcut = true;
        parent->degree--;
        if(parent->parent != NULL)
            cascading(parent, heap);
    }
    else{
        deleteListNode(target);
        target->child = target->parent = NULL;
        free(target);
    }

    //merge
    mergeFib(heap, other);
    return result;
}

void decreaseKeyFib(FibHeap *heap, int targetKey, int newKey){
    Node *target = findNode(heap->min, targetKey);
    if(target == NULL){
        fprintf(stderr, "the key is not in the heap\n");
        exit(-1);
    }
    else if(target->parent == NULL){
        target->data.key = newKey;
        assert(heap->min != NULL);
        if(target->data.key < heap->min->data.key)
            heap->min = target;
        return;
    }

    target->data.key = newKey;
    Node *parent = target->parent;
    parent->degree--;
    parent->childcut = true;

    //put target to root list
    if(target->rs != target){
        target->ls->rs = target->rs;
        target->rs->ls = target->ls;
        parent->child = target->rs;
    }
    else
        parent->child = NULL;
    target->parent = NULL;
    insertNodeToList(heap->min, target);
    
    //cascading
    if(parent->parent != NULL)
        cascading(parent, heap);
    //merge
    FibHeap *temp = initFibHeap();
    mergeFib(heap, temp);
    return;
}

void printHelper(Node *a, int depth){
    if(a == NULL)
        return;

    assert(depth == 0 || a->parent != NULL);        //check parent link
    Node *start = a;
    do{
        for(int i = 0; i < depth; i++)
            printf("    ");
        printf("Key %d, Degree %d\n", a->data.key, a->degree);

        //child
        printHelper(a->child, depth + 1);

        assert(a->rs != NULL);
        a = a->rs;
    }while(a != start);
    return;
}

void printFib(FibHeap *heap){
    if(heap == NULL || heap->min == NULL){
        printf("empty\n");
        exit(-1);
    }

    printf("Heap\n");
    printf("min %d\n", heap->min->data.key);
    printHelper(heap->min, 0);
    return;
}   

void destroyHelper(Node *now){
    if(now == NULL)
        return;
    
    Node *start = now->ls;
    while(now != start){
        destroyHelper(now->child);
        Node *temp = now;
        now = now->rs;
        free(now);
    }
    return;
}

void destroyFib(FibHeap *heap){
    destroyHelper(heap->min);
    free(heap);
    return;
}