#include <stdlib.h>
#include <stdio.h>

#include "ThreadBinaryTree.h"

int main(){
    Element data = {.key = 'A'};
    ThreadBinary *tree = initThread(data);

    Node *root = tree->head->lc;

    data.key = 'B';
    Node *B = insertleft(root, data);
    inorder(tree);

    data.key = 'C';
    Node *C = insertright(root, data);
    inorder(tree);

    data.key = 'D';
    Node *D = insertleft(B, data);
    inorder(tree);

    data.key = 'E';
    Node *E = insertright(B, data);
    inorder(tree);

    data.key = 'F';
    Node *F = insertleft(C, data);
    inorder(tree);

    data.key = 'G';
    Node *G = insertright(C, data);
    inorder(tree);

    data.key = 'H';
    Node *H = insertleft(D, data);
    inorder(tree);

    data.key = 'I';
    Node *I = insertright(D, data);
    inorder(tree);

}
