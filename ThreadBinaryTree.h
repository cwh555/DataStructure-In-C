#ifndef THREAD_H
#define THREAD_H
#include <stdbool.h>
#include "Element.h"

typedef struct node{
    Element data;
    struct node *lc, *rc;
    bool lthread, rthread;
} Node;

typedef struct threadbinary{
    Node *head;
} ThreadBinary;

Node *successor(Node *now);
Node *precessor(Node *now);

void inorder(ThreadBinary *tree);
ThreadBinary* initThread(Element rootData);
Node * insertleft(Node *original, Element data);
Node * insertright(Node *original, Element data);
Element deleteleft(Node *parent, Node *target);
Element deleteright(Node *parent, Node *target);

#endif