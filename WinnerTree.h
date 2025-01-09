/*
sort for list array i.e. need to include linkedlist
only implement for sorting from small to big
*/

#ifndef WINNER_H
#define WINNER_H

#include "LinkedList.h"
#include <stdbool.h>

typedef struct winner WinnerTree;

WinnerTree *createWinnerTree(LinkedList **data, int num);
Element getWinnerTop(WinnerTree *tree);
bool WinnerEmpty(WinnerTree *winner);

void destroyWinner(WinnerTree *winner);

#endif