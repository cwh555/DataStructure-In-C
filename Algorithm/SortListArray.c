#include "SortListArray.h"
#include <stdlib.h>

LinkedList *SortByWinner(LinkedList **array, int num){
    WinnerTree *winner = createWinnerTree(array, num);

    LinkedList *result = initList();

    while(!WinnerEmpty(winner))
        addlistTail(result, getWinnerTop(winner));

    destroyWinner(winner);
    
    return result;
}

LinkedList *SortByLoser(LinkedList **array, int num){
    LoserTree *loser = createLoserTree(array, num);

    LinkedList *result = initList();

    while(!LoserEmpty(loser))
        addlistTail(result, getLoserTop(loser));
    
    destroyLoser(loser);

    return result;
}