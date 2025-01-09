#include "ThreadBinaryTree.h"

#include <stdlib.h>
#include <stdio.h>

//--helper
Node *newNode(Element data){
    Node *new = (Node *)malloc(sizeof(Node));
    new->lc = new->rc = NULL;
    new->lthread = new->rthread = false;
    new->data = data;

    return new;
}

//--helper end

Node *successor(Node *now){
    Node *temp = now->rc;
    if(now->rthread)
        return temp;
    else{
        while(!temp->lthread)
            temp = temp->lc;
        return temp;
    }
}

Node *precessor(Node *now){
    Node *temp = now->lc;
    
    if(now->lthread)
        return temp;
    else{
        while(!temp->rthread)
            temp = temp->rc;
        return temp;
    }
}

void inorder(ThreadBinary *tree){
    Node *now = tree->head->lc;

    while(!now->lthread){
        now = now->lc;
    }

    while(now != tree->head){
        printf("%c ", (char)now->data.key);
        now = successor(now);
    }
    printf("\n");

    return;
}

ThreadBinary* initThread(Element rootData){
    ThreadBinary *tree = (ThreadBinary *)malloc(sizeof(ThreadBinary));
    Element headTemp = {.key = -1};
    tree->head = newNode(headTemp);
    Node *root = newNode(rootData);

    //init
    tree->head->lc = root;
    root->lthread = root->rthread = true;
    root->lc = root->rc = tree->head;

    return tree;
}

Node *insertleft(Node *original, Element data){
    Node *new = newNode(data);

    new->lc = original->lc;
    new->lthread = original->lthread;
    new->rc = original;
    new->rthread = true;
    original->lc = new;
    original->lthread = false;

    if(!new->lthread){
        precessor(new)->rc = new;
    }

    return new;
}

Node *insertright(Node *original, Element data){
    Node *new = newNode(data);

    new->rc = original->rc;
    new->rthread = original->rthread;
    new->lc = original;
    new->lthread = true;

    original->rc = new;
    original->rthread = false;
    if(!new->rthread)
        successor(new)->lc = new;
    
    return new;
}

Element deleteleft(Node *parent, Node *target){
    Element temp = target->data;

    parent->lc = target->lc;
    parent->lthread = target->lthread;
    return temp;
}

Element deleteright(Node *parent, Node *target){
    Element temp = target->data;

    parent->rc = target->rc;
    parent->rthread = target->rthread;
    return temp;
}
