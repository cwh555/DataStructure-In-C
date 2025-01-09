#include "LeftistTree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define SWAP(a, b, t) do{ \
            t = a;  a = b;  b = t;\
        }while(0)

typedef struct node{
    Element data;
    struct node *lc, *rc;    
    int balance;    //record height in HBLT or weight in WBLT
}Node;

struct leftist{
    LeftistType type;
    Node *root;
    void (*merge)(Leftist *treea, Leftist *treeb);
};

//--function declaration
Node *newNode(Element data);

Leftist *initLeftist(LeftistType type);
void insertLeftist(Leftist *tree, Element data);
Element getMin(Leftist *tree);
void printLeftist(Leftist *tree);
void destroyLeftist(Leftist *tree);


Node *HBLTUnion(Node *a, Node *b);
void mergeHBLT(Leftist *treea, Leftist *treeb);

Node *WBLTUnion(Node *a, Node *b);
void mergeWBLT(Leftist *treea, Leftist *treeb);
//--end



//--helper
Node *newNode(Element data){
    Node *new = (Node *)malloc(sizeof(Node));
    new->balance = 0;
    new->data = data;
    new->lc = new->rc = NULL;

    return new;
}


//--end


//--function
Leftist *initLeftist(LeftistType type){
    Leftist *tree = (Leftist *)malloc(sizeof(Leftist));
    tree->root = NULL;
    tree->type = type;

    if(type == HBLT)
        tree->merge = mergeHBLT;
    else{
        assert(type == WBLT);
        tree->merge = mergeWBLT;
    }
    return tree;
}

void insertLeftist(Leftist *tree, Element data){
    Leftist *temp = initLeftist(tree->type);
    Node *new = newNode(data);
    new->balance = 1;
    temp->root = new;
    temp->merge = tree->merge;
    temp->type = tree->type;
    

    tree->merge(tree, temp);
    return;
}


void printHelper(Node *now){
    if(now == NULL)
        return;
    
    printHelper(now->lc);
    printf("%d ", now->data.key);
    printHelper(now->rc);
    return;
}

void printLeftist(Leftist *tree){
    printHelper(tree->root);
    printf("\n");
    return;
}

void destroyHelper(Node *now){
    if(now == NULL)
        return;
    
    destroyHelper(now->lc);
    destroyHelper(now->rc);
    free(now);
    return;
}

void destroyLeftist(Leftist *tree){
    if(tree == NULL)
        return;

    destroyHelper(tree->root);
    free(tree);
    return;
}

Element getMin(Leftist *tree){
    Element result = tree->root->data;
    Leftist *temp1 = initLeftist(tree->type);
    temp1->root = tree->root->lc;

    Leftist *temp2 = initLeftist(tree->type);
    temp2->root = tree->root->rc;

    temp1->merge = temp2->merge;

    free(tree->root);
    tree->merge(temp1, temp2);
    tree->root = temp1->root;
    return result;
}


//--end

//--HBLT
Node *HBLTUnion(Node *a, Node *b){
    if(a->data.key > b->data.key){
        //swap
        Node *temp;
        SWAP(a, b, temp);
    }

    if(a->rc == NULL)
        a->rc = b;
    else
        a->rc = HBLTUnion(a->rc, b);

    //maintain property
    if(a->lc == NULL){
        a->lc = a->rc;
        a->rc = NULL;
    }
    else if(a->lc->balance < a->rc->balance){
        Node *temp;
        SWAP(a->rc, a->lc, temp);
    }

    a->balance = (a->rc == NULL ? 1 : a->rc->balance + 1);
    return a;
}

void mergeHBLT(Leftist *treea, Leftist *treeb){
    if(treea->root == NULL){
        treea->root = treeb->root;
    }
    else if(treeb->root != NULL)
        treea->root = HBLTUnion(treea->root, treeb->root);
    free(treeb);
    return;
}
//--end

//--WBLT
Node *WBLTUnion(Node *a, Node *b){
    if(a == NULL)
        return b;

    if(a->data.key > b->data.key){
        //swap
        Node *temp;
        SWAP(a, b, temp);
    }

    int rb = (a->rc == NULL ? 0 : a->rc->balance) + b->balance;
    int lb = (a->lc == NULL ? 0 : a->lc->balance);

    if(rb > lb){
        Node *temp = a->lc;
        a->lc = WBLTUnion(a->rc, b);
        a->rc = temp;
    }
    else
        a->rc = WBLTUnion(a->rc, b);

    //check
    assert(a->rc == NULL || a->lc->balance >= a->rc->balance);


    a->balance = lb + rb + 1;

    return a;

}


void mergeWBLT(Leftist *treea, Leftist *treeb){
    if(treea->root == NULL){
        treea->root = treeb->root;
    }
    else if(treeb->root != NULL)
        treea->root = WBLTUnion(treea->root, treeb->root);
    free(treeb);
    return;
}
//--end
