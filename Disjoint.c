#include "Disjoint.h"
#include <stdlib.h>

struct disjoint{
    int num;
    int *data;
    int *rank;
};

Disjoint *initDisjoint(int num){
    Disjoint *d = (Disjoint *)malloc(sizeof(Disjoint));
    d->num = num;
    d->data = (int *)malloc(sizeof(int) * num);
    for(int i = 0; i < num; i++)
        d->data[i] = i;
    d->rank = (int *)calloc(num, sizeof(int));
    return d;
}

void merge(Disjoint *disjoint, int a, int b){
    int roota = findRoot(disjoint, disjoint->data[a]);
    int rootb = findRoot(disjoint, disjoint->data[b]);

    if(disjoint->rank[roota] > disjoint->rank[rootb])
        disjoint->data[rootb] = roota;
    else if(disjoint->rank[rootb] > disjoint->rank[roota])
        disjoint->data[roota] = rootb;
    else{
        disjoint->data[rootb] = roota;
        disjoint->rank[roota]++;
    }
    return;
}

bool sameGroup(Disjoint *disjoint, int a, int b){
    return findRoot(disjoint, a) == findRoot(disjoint, b);
}
int findRoot(Disjoint *disjoint, int a){
    if(disjoint->data[a] == a)
        return a;
    
    return disjoint->data[a] = findRoot(disjoint, disjoint->data[a]);
}

void destroyDisjoint(Disjoint *disjoint){
    free(disjoint->data);
    free(disjoint);
}