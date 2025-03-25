#include "AVLTree.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef struct node{
    struct node *lc, *rc;
    Element data;
    int balance;
}Node;

struct avl{
    struct node *root;
    int dataNum;
};

//--declaration
void rightRotate(Node *target);
void leftRotate(Node *target);
//--end

//--helper
void rightRotate(Node *target){
    Node *child = target->lc;
    assert(child != NULL);

    target->lc = child->rc;
    child->rc = target;
    return;
}

void leftRotate(Node *target){
    Node *child = target->rc;
    assert(child != NULL);

    target->rc = child->lc;
    child->lc = target;
    return;
}
//-end

AVLTree *initAVL(){
    AVLTree *tree = (AVLTree *)malloc(sizeof(AVLTree));
    tree->root =  NULL;
    tree->dataNum = 0;
    return tree;
}

void 

void insertAVL(AVLTree *tree, Element data){
    assert(tree == NULL);




}

Element findAVL(AVLTree *tree, int key){
    Node *now = tree->root;

    while(now != NULL){
        if(now->data.key == key)
            return now->data;
        else if(now->data.key > key)
            now = now->lc;
        else
            now = now->rc;
    }    

    //not find
    Element temp = {.key = -1};
    return temp;
}

Element deleteAVL(AVLTree *tree, int key);

void destroyHelper(Node *a){
    if(a == NULL)
        return;
    
    destroyHelper(a->lc);
    destroyHelper(a->rc);
    free(a);
    return;
}

void destroyAVL(AVLTree *tree){
    destroyHelper(tree->root);
    free(tree);
    return;
}

void printHelper(Node *now){
    if(now == NULL)
        return;
    
    //check
    assert(now->balance <= 1 && now->balance >= -1);

    printHelper(now->lc);
    printf("%d ", now->data.key);
    printHelper(now->rc);
    return;
}

void printAVL(AVLTree *tree){
    printf("AVL Tree\n");
    printHelper(tree->root);
    return;
}
