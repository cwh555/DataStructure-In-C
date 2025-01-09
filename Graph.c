#include "Graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct adjMatrix{
    int **matrix;
    int size;
    int noEdgeWeight;
}AdjMatrix;

typedef struct edgeListnode{
    int start, end;
    int weight;
    struct edgeListnode *next;
}EdgeListNode;

typedef struct edgeList{
    int vertexNum;
    int num;
    EdgeListNode *head;
}EdgeList;

typedef struct adjlistnode{
    int from, to;
    int weight;
    struct adjlistnode *in;     //... -> from
    struct adjlistnode *out;    //in -> ...
}AdjListNode;

typedef struct adjlist{
    int size;
    AdjListNode *list;
}AdjList;

struct graph{
    int vertexNum, edgeNum;
    void *edge;
    bool (*addEdge)(void *edge, int start, int end, int wieght);
    void (*printGraph)(void *edge);
    void (*destroyEdge)(void *edge);
    int **(*getEdge)(void *edge, int num);
};

//--adjacency matrix
void initAdjMatrix(Graph *g, int noEdgeWeight){
    g->edge = (AdjMatrix *)malloc(sizeof(AdjMatrix));
    AdjMatrix *m = (AdjMatrix *)g->edge;

    m->size = g->vertexNum;
    m->noEdgeWeight = noEdgeWeight;
    m->matrix = (int **)malloc(sizeof(int *) * g->vertexNum);
    for(int i = 0; i < g->vertexNum; i++){
        m->matrix[i] = (int *)malloc(sizeof(int) * g->vertexNum);
        for(int j = 0; j < g->vertexNum; j++)
            m->matrix[i][j] = noEdgeWeight;
    }
    return;
}

bool addAdjMatrix(void *edge, int start, int end, int weight){
    AdjMatrix *e = (AdjMatrix *)edge;
    if(start < 0 || end < 0 || start >= e->size || end >= e->size)
        return false;
    
    e->matrix[start][end] = weight;
    return true;
}

void printAdjMatrix(void *edge){
    AdjMatrix *e = (AdjMatrix *)edge;
    for(int i = 0; i < e->size; i++){
        printf("%d : ", i);
        for(int j = 0; j < e->size; j++){
            if(e->matrix[i][j] != e->noEdgeWeight)
                printf("%d ", j);
        }
        printf("\n");
    }
    return;
}

void destroyAdjMatrix(void *edge){
    AdjMatrix *e = (AdjMatrix *)edge;
    for(int i = 0; i < e->size; i++)
        free(e->matrix[i]);
    free(e->matrix);
    return;
}

int **getAdjMatrix(void *edge, int num){
    AdjMatrix *e = (AdjMatrix *)edge;
    int **record = (int **)malloc(sizeof(int *) * num);
    for(int i = 0; i < num; i++)
        record[i] = (int *)malloc(sizeof(int) * 3);

    int count = 0;

    for(int i = 0; i < e->size && count < num; i++){
        for(int j = 0; j < e->size && count < num; j++){
            if(e->matrix[i][j] != e->noEdgeWeight){
                record[count][0] = i, record[count][1] = j, record[count][2] = e->matrix[i][j];
                count++;
            }
        }
    }
    return record;
}
//--end

//--adjacency list
void initAdjList(Graph *g){
    g->edge = malloc(sizeof(AdjList));
    AdjList *l = (AdjList *)g->edge;
    l->size = g->vertexNum;
    l->list = (AdjListNode *)malloc(sizeof(AdjListNode) * g->vertexNum);
    for(int i = 0; i < l->size; i++){
        l->list[i].in = l->list[i].out = NULL;
        l->list[i].from = l->list[i].to = i;
    }
    return;
}

bool addAdjList(void *edge, int start, int end, int weight){
    AdjList *e = (AdjList *)edge;
    if(start < 0 || end < 0 || start >= e->size || end >= e->size)
        return false;

    AdjListNode *new = (AdjListNode *)malloc(sizeof(AdjListNode));
    new->from = start;
    new->to = end;
    new->weight = weight;
    
    //insert for out
    AdjListNode *now = e->list[start].out;
    AdjListNode *before = &e->list[start];
    while(now != NULL && now->to < end){
        before = now;
        now = now->out;
    }
    //insert after before
    before->out = new;
    new->out = now;

    //insert for in
    now = e->list[end].in;
    before = &e->list[end];
    while(now != NULL && now->from < start){
        before = now;
        now = now->in;
    }
    //insert after before
    before->in = new;
    new->in = now;

    return true;
}

void printAdjList(void *edge){
    AdjList *e = (AdjList *)edge;
    printf("out\n");
    for(int i = 0; i < e->size; i++){
        printf("%d : ", i);
        AdjListNode *now = e->list[i].out;
        while(now != NULL){
            printf("%d ", now->to);
            now = now->out;
        }
        printf("\n");
    }

    printf("in\n");
    for(int i = 0; i < e->size; i++){
        printf("%d : ", i);
        AdjListNode *now = e->list[i].in;
        while(now != NULL){
            printf("%d ", now->from);
            now = now->in;
        }
        printf("\n");
    }

    return;
}

void destroyAdjList(void *edge){
    AdjList *l = (AdjList *)edge;
    for(int i = 0; i < l->size; i++){
        AdjListNode *now = l->list[i].out;
        while(now != NULL){
            AdjListNode *temp = now;
            now = now->out;
            free(temp);
        }
    }

    free(l->list);
    return;
}

int** getAdjList(void *edge, int num){
    AdjList *e = (AdjList *)edge;
    int **record = (int **)malloc(sizeof(int *) * num);
    int count = 0;

    for(int i = 0; i < e->size && count < num; i++){
        AdjListNode *now = e->list[i].out;
        while(now != NULL){
            record[count] = (int *)malloc(sizeof(int[3]));
            record[count][0] = now->from, record[count][1] = now->to, record[count][2] = now->weight;
            count++;
            now = now->out;
        }
    }

    return record;
}
//--end

//--edge List
void initEdgeList(Graph *g){
    g->edge = malloc(sizeof(EdgeList));
    EdgeList *e = (EdgeList *)g->edge;
    e->vertexNum = g->vertexNum;
    e->num = 0;

    //init header
    e->head = (EdgeListNode *)malloc(sizeof(EdgeListNode));
    e->head->start = e->head->end = e->head->weight = -1;
    e->head->next = NULL;
    return;
}

bool addEdgeList(void *edge, int start, int end, int weight){
    EdgeList *e = (EdgeList *)edge;
    if(start < 0 || end < 0 || start >= e->vertexNum || end >= e->vertexNum)
        return false;
    
    EdgeListNode *new = (EdgeListNode *)malloc(sizeof(EdgeListNode));
    new->start = start, new->end = end;
    new->weight = weight;

    EdgeListNode *before = e->head;
    EdgeListNode *now = e->head->next;
    while(now != NULL && 
        (now->start < start || (now->start == start && now->end < end))){
        before = now;
        now = now->next;
    }
    //insert after before
    before->next = new;
    new->next = now;    
    return true;
}

void printEdgeList(void *edge){
    EdgeList *e = (EdgeList *)edge;
    EdgeListNode *now = e->head->next;
    while(now != NULL){
        printf("%d %d\n", now->start, now->end);
        now = now->next;
    }
    return;
}

void destroyEdgeList(void *edge){
    EdgeList *e = (EdgeList *)edge;

    EdgeListNode *now = e->head;
    while(now != NULL){
        EdgeListNode *temp = now;
        now = now->next;
        free(temp);
    }
    return;
}

int **getEdgeList(void *edge, int num){
    EdgeList *e = (EdgeList *)edge;

    int **record = (int **)malloc(sizeof(int *) * num);
    int count = 0;

    EdgeListNode *now = e->head->next;
    while(now != NULL){
        record[count] = (int *)malloc(sizeof(int[3]));
        record[count][0] = now->start, record[count][1] = now->end, record[count][2] = now->weight;
        count++;
        now = now->next;
    }
    return record;
}

//--end


//--all function
//tpye: ADJMATRIX, ADJLIST, EDGELIST
Graph *initGraph(int vertexNum, GraphType type, int noEdgeWeight){
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->vertexNum = vertexNum;
    graph->edgeNum = 0;

    switch(type){
        case ADJMATRIX:
            initAdjMatrix(graph, noEdgeWeight);
            graph->addEdge = addAdjMatrix;
            graph->printGraph = printAdjMatrix;
            graph->destroyEdge = destroyAdjMatrix;
            graph->getEdge = getAdjMatrix;
            break;
        case ADJLIST:
            initAdjList(graph);
            graph->addEdge = addAdjList;
            graph->printGraph = printAdjList;
            graph->destroyEdge = destroyAdjList;
            graph->getEdge = getAdjList;
            break;
        case EDGELIST:
            initEdgeList(graph);
            graph->addEdge = addEdgeList;
            graph->printGraph = printEdgeList;
            graph->destroyEdge = destroyEdgeList;
            graph->getEdge = getEdgeList;
            break;
        default:
            fprintf(stderr, "fail graph type\n");
            exit(-1);
    }

    return graph;
}

void printGraph(Graph *g){
    g->printGraph(g->edge);
    return;
}

void addEdge(Graph *g, int start, int end, int weight){
    if(g->addEdge(g->edge, start, end, weight))
        g->edgeNum++;

    return;
}

void destroyGraph(Graph *g){
    g->destroyEdge(g->edge);
    free(g->edge);
    free(g);
    return;
}

int getVertexNum(Graph *g){
    return g->vertexNum;
}

int getEdgeNum(Graph *g){
    return g->edgeNum;
}

//return tuple (start, end, weight)
int **getAllEdge(Graph *g){
    return g->getEdge(g->edge, g->edgeNum);
}

//--end