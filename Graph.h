/*
store edge by three methods, adjacency matrix, adjacency list, edge list
*/

#ifndef GRAPH_H
#define GRAPH_H

typedef struct graph Graph;

typedef enum{
    ADJMATRIX, ADJLIST, EDGELIST
}GraphType;

Graph *initGraph(int vertexNum, GraphType type, int noEdgeWeight);
void printGraph(Graph *g);
void addEdge(Graph *g, int start, int end, int weight);
void destroyGraph(Graph *g);

int getVertexNum(Graph *g);
int getEdgeNum(Graph *g);

//return tuple (start, end, weight)
int **getAllEdge(Graph *g);
#endif