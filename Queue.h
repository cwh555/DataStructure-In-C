#ifndef QUEUE_H
#define QUEUE_H
#include <stdlib.h>
#include <stdbool.h>

typedef struct queue Queue;

Queue *initQueue(int maxsize);
bool enqueue(Queue *q, int element);
int dequeue(Queue *q);
bool Queueisempty(Queue *q);
bool Queueisfull(Queue *q);

int getNum(Queue *q);

void destroyQueue(Queue *q);

#endif