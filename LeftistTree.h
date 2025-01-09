/*
there are two type HBLT and WBLT
Leftist tree support merge and getMin
*/


#ifndef LEFTIST_H
#define LEFTIST_H

#include "Element.h"
#include <stdbool.h>

typedef struct leftist Leftist;

typedef enum{
    HBLT, WBLT
}LeftistType;

Leftist *initLeftist(LeftistType type);
void insertLeftist(Leftist *tree, Element data);
Element getMin(Leftist *tree);
void printLeftist(Leftist *tree);
void destroyLeftist(Leftist *tree);



#endif