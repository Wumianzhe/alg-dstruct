#include "list.h"
#include <stdbool.h>
#include <stdio.h>
#ifndef GRAPH_H
#define GRAPH_H 1

typedef struct {
    int size;
    list_t** links;
} graph_t;

void graphDelete(graph_t* graph);
graph_t* graphRead(FILE* in);
int hamiltonPath(graph_t* graph, FILE* out);
int* hamiltonPathRecursive(graph_t* graph, int depth, int index, bool* flags);
int hamiltonRecursiveWrap(graph_t* graph, FILE* out);

#endif
