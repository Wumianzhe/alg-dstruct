#ifndef GRAPH_H
#define GRAPH_H
#include "queue.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int size;
    queue_t** links; // array of any lists can be used and queues are already defined
} graph_t;

void graphDelete(graph_t* graph);
graph_t* graphRead(FILE* in);
int breadthFirstSearch(graph_t* graph, FILE* out);

#ifdef __cplusplus
}
#endif

#endif
