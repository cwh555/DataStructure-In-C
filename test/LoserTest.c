#include <stdlib.h>
#include <stdio.h>

#include "LinkedList.h"
#include "LoserTree.h"

int main(){
    LinkedList **array = (LinkedList **)malloc(sizeof(LinkedList *) * 6);
    int data[12] = {15, 10, 20, 9, 20, 20, 15, 6, 15, 8, 18, 17};
    
    for(int i = 0; i < 6; i++){
        array[5 - i] = initList();
        for(int j = 0; j < 2; j++){
            Element new = {.key = data[i * 2 + j]};
            addlistHead(array[5 - i], new);
        }
    }

    for(int i = 0; i < 6; i++){
        printList(array[i]);
    }

    LoserTree *loser = createLoserTree(array, 6);
    while(!LoserEmpty(loser)){
        printf("%d ", getLoserTop(loser).key);
    }
    printf("\n");
}
