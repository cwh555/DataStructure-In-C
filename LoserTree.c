#include "LoserTree.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//--data structure
struct loser{
    int dataNum;
    LinkedList **data;
    
    int *tree;
    int treeSize;
};
//--end

//--helper
inline static int getlc(int index){
    return 2 * index;
}

inline static int getrc(int index){
    return 2 * index + 1;
}

inline static int getparent(int index){
    return index / 2;
}
//--end


LoserTree *createLoserTree(LinkedList **data, int num){
    LoserTree *loser = (LoserTree *)malloc(sizeof(LoserTree));
    
    //complement num to 2^n
    loser->dataNum = 1;
    while(loser->dataNum < num)
        loser->dataNum *= 2;

    //init loser->data
    loser->data = (LinkedList **)malloc(sizeof(LinkedList *) * loser->dataNum);
    for(int i = 0; i < loser->dataNum; i++){
        if(i < num)
            loser->data[i] = data[i];
        else{
            //init a linked list with INTMAX(which will not affect result)
            loser->data[i] = initList();
            Element temp = {.key = INT32_MAX};
            addlistHead(loser->data[i], temp);
        }
    }

    loser->treeSize = loser->dataNum * 2;
    loser->tree = (int *)malloc(sizeof(int) * loser->treeSize);
    for(int i = 0; i < loser->treeSize; i++)
        loser->tree[i] = -1;

    //init leaf node and internal node
    for(int i = 0; i < loser->dataNum; i++){
        int indexNow = loser->treeSize - 1 - i;
        loser->tree[indexNow] = i;
        
        int winner = i;
        while(indexNow > 0){
            int parent = getparent(indexNow);

            if(loser->tree[parent] == -1){
                loser->tree[parent] = winner;
                break;
            }
            else if(getlistTop(loser->data[loser->tree[parent]]).key <\
                    getlistTop(loser->data[winner]).key){
                //i lose
                int temp = winner;
                winner = loser->tree[parent];
                loser->tree[parent] = temp;
            }

            indexNow = parent;
        }
    }

    return loser;
}


Element getLoserTop(LoserTree *loser){
    Element top = getlistTop(loser->data[loser->tree[0]]);
    if(top.key == INT32_MAX){
        fprintf(stderr, "loser tree is empty\n");
        exit(-1);
    }

    deletelistTop(loser->data[loser->tree[0]]);
    if(listEmpty(loser->data[loser->tree[0]])){
        Element temp = {.key = INT32_MAX};
        addlistHead(loser->data[loser->tree[0]], temp);
    }

    //update from leaf
    int indexNow = loser->treeSize - 1 - loser->tree[0];
    int winner = loser->tree[0];

    while(indexNow > 0){
        int parent = getparent(indexNow);

        if(getlistTop(loser->data[loser->tree[parent]]).key <\
            getlistTop(loser->data[winner]).key){
            
            int temp = winner;
            winner = loser->tree[parent];
            loser->tree[parent] = temp;
        }

        indexNow = parent;
    }
    loser->tree[0] = winner;

    return top;
}

bool LoserEmpty(LoserTree *loser){
    return getlistTop(loser->data[loser->tree[0]]).key == INT32_MAX;
}

void destroyLoser(LoserTree *loser){
    for(int i = 0; i < loser->dataNum; i++)
        destroylist(loser->data[i]);
    
    free(loser->data);
    free(loser->tree);
    free(loser);

    return;
}