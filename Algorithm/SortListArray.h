/*
sort list array from small to big
each array must be sorted before (from small to big)
*/


#ifndef SORTLIST_H
#define SORTLIST_H
#include "WinnerTree.h"
#include "LoserTree.h"
#include "LinkedList.h"

LinkedList *SortByWinner(LinkedList **array, int num);
LinkedList *SortByLoser(LinkedList **array, int num);



#endif