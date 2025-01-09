#include "WinnerTree.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

//--dataStruct
struct winner{
    //list store in leaf
    LinkedList **data;
    int dataNum;

    int *tree;
    int treeSize;
};

//--end

//--helper
inline static int getlc(int index){
    return index * 2 + 1;
}

inline static int getrc(int index){
    return index * 2 + 2;
}

inline static int getparent(int index){
    return (index - 1) / 2;
}
//--end



WinnerTree *createWinnerTree(LinkedList **data, int num){
    WinnerTree *winner = (WinnerTree *)malloc(sizeof(WinnerTree));

    //complement num to the nearest 2^n
    winner->dataNum = 1;
    while(winner->dataNum < num)
        winner->dataNum *= 2;


    //init list in tree
    winner->data = (LinkedList **)malloc(sizeof(LinkedList *) * winner->dataNum);
    for(int i = 0; i < winner->dataNum; i++){
        if(i < num)
            winner->data[i] = data[i];
        else{
            //init a linked list with INTMAX(which will not affect result)
            winner->data[i] = initList();
            Element temp = {.key = INT32_MAX};
            addlistHead(winner->data[i], temp);
        }
    }

    //init tree
    winner->treeSize = winner->dataNum * 2 - 1;
    winner->tree = (int *)malloc(sizeof(int) * winner->treeSize);

    //init leaf
    int treeIndex = winner->treeSize - 1;
    for(int i = 0; i < winner->dataNum; i++, treeIndex--)
        winner->tree[treeIndex] = i;

    //init internal node
    for(; treeIndex >= 0; treeIndex--){
        int lcList = winner->tree[getlc(treeIndex)];
        int rcList = winner->tree[getrc(treeIndex)];

        if(getlistTop(winner->data[lcList]).key < getlistTop(winner->data[rcList]).key)
            winner->tree[treeIndex] = lcList;
        else
            winner->tree[treeIndex] = rcList;
    }

    return winner;
}

Element getWinnerTop(WinnerTree *winner){
    Element top = getlistTop(winner->data[winner->tree[0]]);
    if(top.key == INT32_MAX){
        fprintf(stderr, "winner tree is empty\n");
        exit(-1);
    }
    
    deletelistTop(winner->data[winner->tree[0]]);
    if(listEmpty(winner->data[winner->tree[0]])){
        Element temp = {.key = INT32_MAX};
        addlistHead(winner->data[winner->tree[0]], temp);
    }

    //update leaf
    int indexNow = winner->treeSize - 1 - winner->tree[0];
    while(indexNow != 0){
        indexNow = getparent(indexNow);

        int lcList= winner->tree[getlc(indexNow)];
        int rcList = winner->tree[getrc(indexNow)];

        if(getlistTop(winner->data[lcList]).key < getlistTop(winner->data[rcList]).key)
            winner->tree[indexNow] = lcList;
        else
            winner->tree[indexNow] = rcList;
    }

    return top;
}

bool WinnerEmpty(WinnerTree *winner){
    return getlistTop(winner->data[winner->tree[0]]).key == INT32_MAX;
}

void destroyWinner(WinnerTree *winner){
    for(int i = 0; i < winner->dataNum; i++)
        destroylist(winner->data[i]);

    free(winner->data);
    free(winner->tree);
    free(winner);

    return;
}