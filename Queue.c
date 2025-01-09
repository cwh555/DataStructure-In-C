#include "Queue.h"

//--dataStructure
struct queue{
    int maxsize;
    int rear, front;
    int *data;
};
//--dataStructure end

//--function
Queue *initQueue(int maxsize){
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->data = (int *)malloc(sizeof(int) * (maxsize + 1));
    q->front = q->rear = 0;
    q->maxsize = maxsize;

    return q;
}

bool enqueue(Queue *q, int element){
    if(Queueisfull(q))
        return false;
    
    q->data[q->rear] = element;
    q->rear = (q->rear + 1) % q->maxsize;

    return true;
}

int dequeue(Queue *q){
    if(Queueisempty(q))
        return -1;
    
    int element = q->data[q->front];
    q->front = (q->front + 1 + q->maxsize) % q->maxsize;
    return element;
}

bool Queueisempty(Queue *q){
    return q->front == q->rear;
}

bool Queueisfull(Queue *q){
    return (q->rear + 1) % q->maxsize == q->front;
}

int getNum(Queue *q){
    return (q->rear - q->front + q->maxsize) % q->maxsize;
}

void destroyQueue(Queue *q){
    free(q->data);
    free(q);
    return;
}