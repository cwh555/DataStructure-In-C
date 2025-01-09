#ifndef LOSER_H
#define LOSER_H

#include "LinkedList.h"
#include <stdbool.h>

typedef struct loser LoserTree;

LoserTree *createLoserTree(LinkedList **data, int num);
Element getLoserTop(LoserTree *loser);
bool LoserEmpty(LoserTree *loser);

void destroyLoser(LoserTree *loser);



#endif