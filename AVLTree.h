#ifndef AVL_H
#define AVL_H

#include "Element.h"

typedef struct avl AVLTree;

AVLTree *initAVL();
void insertAVL(AVLTree *tree, Element data);
Element findAVL(AVLTree *tree, int key);
Element deleteAVL(AVLTree *tree, int key);

void destroyAVL(AVLTree *tree);
void printAVL(AVLTree *tree);

#endif