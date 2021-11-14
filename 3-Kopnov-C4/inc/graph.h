#ifndef GRAPH_H
#define GRAPH_H
#include "queue.h"
#include <stdio.h>

typedef struct {
    int size;
    queue_t** links; // array of any lists can be used and queues are already defined
} graph_t;

graph_t* graphCreate(int size);
void graphDelete(graph_t* graph);
void graphRead(graph_t* graph, FILE* in);
int breadthFirstSearch(graph_t* graph, FILE* out);

#endif
