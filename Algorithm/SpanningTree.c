#include "algorithm.h"
#include "Disjoint.h"
#include <stdlib.h>
#include <stdio.h>


//--helper
inline static int Kruskalcompare(const void *a, const void *b){
    int *data1 = *(int **)a;
    int *data2 = *(int **)b;

    if(data1[2] > data2[2])
        return 1;
    else
        return -1;
}

//--end

bool Kruskal(Graph *g, int *cost, Graph **spanning){
    //init spanning
    *spanning = initGraph(getVertexNum(g), EDGELIST, -1);
    *cost = 0;

    //get all edge
    int **edge = getAllEdge(g);

    //sort edge from small to big
    qsort(edge, getEdgeNum(g), sizeof(int*), Kruskalcompare);

    //disjoint
    Disjoint *disjoint = initDisjoint(getVertexNum(g));
    for(int indexNow = 0; indexNow < getEdgeNum(g) && getEdgeNum(*spanning) < getVertexNum(g) - 1;
            indexNow++){
        int vertex1 = edge[indexNow][0], vertex2 = edge[indexNow][1];
        if(!sameGroup(disjoint, vertex1, vertex2)){
            addEdge(*spanning, vertex1, vertex2, edge[indexNow][2]);
            (*cost) += edge[indexNow][2];
            merge(disjoint, vertex1, vertex2);
        }
    }
    
    destroyDisjoint(disjoint);
    if(getEdgeNum(*spanning) < getVertexNum(g) - 1){
        destroyGraph(*spanning);
        return false;
    }
    else
        return true;
}