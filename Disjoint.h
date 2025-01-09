#ifndef DISJOINT_H
#define DISJOINT_H

#include <stdbool.h>

typedef struct disjoint Disjoint;

Disjoint *initDisjoint(int num);
void merge(Disjoint *disjoint, int a, int b);
bool sameGroup(Disjoint *disjoint, int a, int b);
int findRoot(Disjoint *disjoint, int a);

void destroyDisjoint(Disjoint *disjoint);

#endif