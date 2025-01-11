#include "PairingHeap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#define MINKEY INT32_MIN
#define SWAP(a, b, temp) do{\
            temp = a; a = b; b = temp;\
        }while(0)

typedef struct node{
    Element data;
    struct node *ls, *rs;   //the leftest child set ls = parent
    struct node *child;
}Node;

struct pairing{
    Node *min;
    PAIRTYPE type;
    void (*mergePass)(PairHeap *heap);
};

//--helper dataStructure
typedef struct vector{
    int size;
    int dataNum;
    Node **data;
}Vector;

Vector *initVector(int size){
    Vector *vector = (Vector *)malloc(sizeof(Vector));
    vector->size = size;
    vector->dataNum = 0;
    vector->data = (Node **)malloc(sizeof(Node *) * size);
    return vector;
}

void pushVector(Vector *vector, Node *newData){
    if(vector->size == vector->dataNum){
        vector->size *= 2;
        vector->data = (Node **)realloc(vector->data, sizeof(Node *) * vector->size);
        assert(vector->data != NULL);
    }

    vector->data[vector->dataNum++] = newData;
    return;
}

void destroyVector(Vector *vector){
    free(vector->data);
    free(vector);
}

typedef struct queue{
    int size;
    int front, rear;
    int dataNum;
    Node **data;
}Queue;

Queue *initQueue(int size){
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    queue->dataNum = 0;
    queue->size = size;
    queue->front = queue->rear = 0;
    queue->data = (Node **)malloc(sizeof(Node *) * size);
    return queue;
}

void enqueue(Queue *queue, Node *newData){
    if(queue->size - 1 == queue->dataNum){
        assert(queue->front == 0);
        queue->size *= 2;
        queue->data = (Node **)realloc(queue->data, sizeof(Node *) * queue->size);
        assert(queue->data != NULL);
    }

    queue->dataNum++;
    queue->data[queue->rear] = newData;
    queue->rear = (queue->rear + 1) % queue->size;
    return;
}

Node *dequeue(Queue *queue){
    Node *data = queue->data[queue->front];
    queue->front = (queue->front + 1) % queue->size;
    queue->dataNum--;
    return data;
}

void destroyQueue(Queue *queue){
    free(queue->data);
    free(queue);
    return;
}

//--end

//--function declaration
void twoPass(PairHeap *heap);
void multiPass(PairHeap *heap);
//--end


Node *newNode(Element data){
    Node *new = (Node *)malloc(sizeof(Node));
    new->data = data;
    new->ls = new->rs = new->child = NULL;
    return new;
}

PairHeap *initPair(PAIRTYPE type){
    PairHeap *heap = (PairHeap *)malloc(sizeof(PairHeap));
    heap->type = type;
    if(type == TWOPASS)
        heap->mergePass = twoPass;
    else
        heap->mergePass = multiPass;

    heap->min = NULL;
    return heap;
}

Node *getParent(Node *a){
    Node *temp = a;
    while(a->ls->child != a)
        a = a->ls;
    return a;
}

Node *mergeNode(Node *a, Node *b){
    Node *buffer;
    if(a == NULL)
        return b;
    else if(b == NULL)
        return a;
    else if(a->data.key > b->data.key)
        SWAP(a, b, buffer);

    if(a->child != NULL){
        b->rs = a->child;
        a->child->ls = b;
    }

    b->ls = a;
    a->child = b;
    return a;
}

//merge to heap and free other
void mergePair(PairHeap *heap, PairHeap *other){
    heap->min = mergeNode(heap->min, other->min);
    free(other);
    return;
}

void insertPair(PairHeap *heap, Element data){
    PairHeap *other = initPair(heap->type);
    Node *new = newNode(data);

    other->min = new;
    mergePair(heap, other);
    return;
}

void twoPass(PairHeap *heap){
    Vector *record = initVector(100);
    Node *now = heap->min->child;
    if(now == NULL){
        free(heap->min);
        heap->min = NULL;
        return;
    }
    assert(now->ls->child == now);  //check parent link
    while(now != NULL){
        Node *another = now->rs;
        if(another == NULL){
            now->ls = now->rs = NULL;
            pushVector(record, now);
            break;
        }

        //break link
        Node *next = another->rs;
        now->ls = now->rs = NULL;
        another->ls = another->rs = NULL;

        now = mergeNode(now, another);
        pushVector(record, now);
        now = next;
    }
    now = record->data[0];
    for(int i = 1; i < record->dataNum; i++){
        assert(now->ls == NULL && now->rs == NULL);
        now = mergeNode(now, record->data[i]);
    }

    //check link
    assert(heap->min->rs == NULL && heap->min->ls == NULL);
    //free
    heap->min->child = NULL;
    free(heap->min);

    heap->min = now;
    destroyVector(record);
    return;
}

void multiPass(PairHeap *heap){
    Queue *queue = initQueue(100);
    Node *now = heap->min->child;
    if(now == NULL){
        free(heap->min);
        heap->min = NULL;
        return;
    }
    assert(now->ls == heap->min);
    while(now != NULL){
        Node *next = now->rs;
        now->ls = now->rs = NULL;
        enqueue(queue, now);
        now = next;
    }

    while(queue->dataNum > 1){
        Node *first = dequeue(queue);
        Node *second = dequeue(queue);
        assert(first->ls == NULL && first->rs == NULL);

        first = mergeNode(first, second);
        enqueue(queue, first);
    }

    assert(heap->min->ls == NULL && heap->min->rs == NULL);
    heap->min->child = NULL;
    free(heap->min);
    heap->min = dequeue(queue);
    destroyQueue(queue);
    return;
}

Element deleteMinPair(PairHeap *heap){
    Element result = heap->min->data;
    heap->mergePass(heap);
    return result;
}

Element deletePair(PairHeap *heap, int key){
    decreaseKeyPair(heap, key, MINKEY);
    return deleteMinPair(heap);
}

Node *find(Node *now, int target){
    if(now == NULL)
        return NULL;
    
    while(now != NULL){
        if(now->data.key == target)
            return now;
        
        Node *result = find(now->child, target);
        if(result != NULL)
            return result;
        
        now = now->rs;
    }
    return NULL;
}

void decreaseKeyPair(PairHeap *heap, int targetKey, int newKey){
    Node *target = find(heap->min, targetKey);
    if(target == NULL){
        fprintf(stderr, "key is not in the heap\n");
        exit(-1);
    }
    else if(target == heap->min){
        heap->min->data.key = newKey;
        return;
    }

    //break target
    if(target->ls->child == target)
        target->ls->child = target->rs;
    else
        target->ls->rs = target->rs;

    if(target->rs != NULL)
        target->rs->ls = target->ls;
    
    target->ls = target->rs = NULL;
    target->data.key = newKey;

    heap->min = mergeNode(heap->min, target);
    return;
}

void printHelper(Node *now, int depth){
    if(now == NULL)
        return;
    
    while(now != NULL){
        for(int i = 0; i < depth; i++)
            printf("  ");

        if(now->child != NULL){
            assert(now->child->ls == now);

            printf("root %d start\n", now->data.key);
            printHelper(now->child, depth + 1);
        
            for(int i = 0; i < depth; i++)
                printf("  ");
            printf("root %d end\n", now->data.key);
        }
        else
            printf("node %d\n", now->data.key);
        now = now->rs;
    }
    return;
}

void printPair(PairHeap *heap){
    if(heap->min == NULL){
        printf("the heap is empty\n");
        return;
    }

    printf("heap\n");
    printHelper(heap->min, 0);
    return;
}

void destroyHelper(Node *now){
    if(now == NULL)
        return;
    
    while(now != NULL){
        Node *next = now->rs;
        destroyHelper(now->child);
        free(now);
        now = next;
        return;
    }
}

void destroyPair(PairHeap *heap){
    destroyHelper(heap->min);
    free(heap);
    return;
}