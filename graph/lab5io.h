#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <limits.h>
#include <float.h>
#include <math.h>

typedef struct vertex{ // вершина
    double x, y;
    char* name;
    struct edge* edge; // список исходящих ребер
    struct vertex* prev; // укзаатель на предыдущую вершину
    double visit; // BFS: если посещено - 1, нет - 0. BLMNFORD: переменная для расстояний
}vertex;

typedef struct edge{ // ребро
    struct vertex* pointvertx; // вершина куда указывает ребро
    struct edge* nextedge;
    double weight;  // вес ребра
}edge;

typedef struct adjencylist{
    struct vertex* array_vertices;
    int num_vertices;
}adjencylist;

typedef struct Queue{
    struct vertex** mass;
    int tail, head;
}Queue;

typedef struct Stack{
    struct vertex** mass;
    int top;
}Stack;

int  dialog();
int  getInt(int *a);
char* readline();

void add_vertex(adjencylist* adlist);
void delete_vertex(adjencylist* adlist);
void reorganize_adlist(adjencylist* adlist);
void add_edge(adjencylist* adlist);
void delete_edge(adjencylist* adlist);
void show_adjancylist(adjencylist* adlist);

//BFS
void enqueue(Queue* queue, vertex** scan);
void dequeue(Queue* queue, vertex** scan);
void BFS(adjencylist* adlist);

void graphviz(adjencylist* adlist);

void blmnford_shortpath(adjencylist* adlist);

void DFS_acycle(vertex* scan, int* detect);
int check_acyclicity(adjencylist* adlist, int* detect);

void push(Stack* stack, vertex** scan);
void DFS_topologic(vertex* scan, Stack* stack);
void topologic_sort(adjencylist* adlist);
void printfsortGraph(Stack* stack);

void clear_graph(adjencylist* adlist);

void work_withroutes(adjencylist* adlist);
void add_vert_route(adjencylist* adlist, char* name_vert);
void add_edge_route(adjencylist* adlist, char* vertex1, char* vertex2, double weight);