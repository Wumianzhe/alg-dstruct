#include "graph.h"
#include "list.h"
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

void graphDelete(graph_t* graph) {
    if (graph) {
        for (int i = 0; i < graph->size; i++) {
            listDelete(graph->links[i]);
        }
        free(graph->links);
        free(graph);
    }
}

graph_t* graphRead(FILE* in) {
    int first, second;
    int size;
    fscanf(in, "%d\n", &size);
    graph_t* graph = malloc(sizeof(graph_t));
    if (graph) {
        graph->size = size;
        graph->links = malloc(size * sizeof(list_t));
        if (!graph->links) {
            free(graph);
            return NULL;
        }
        for (int i = 0; i < size; i++) {
            graph->links[i] = listCreate();
            if (!graph->links[i]) {
                for (int k = 0; k < i; k++) {
                    listDelete(graph->links[k]);
                }
                free(graph);
                return NULL;
            }
        }
        while (fscanf(in, "%d %d\n", &first, &second) > 0) {
            insertLast(graph->links[first - 1], second - 1);
            insertLast(graph->links[second - 1], first - 1);
            if (errno) {
                graphDelete(graph);
                return NULL;
            }
        }
    }
    return graph;
}

int hamiltonPath(graph_t* graph, FILE* out) {
    bool* flags = calloc(graph->size, sizeof(bool));
    if (!flags) {
        return 0;
    }
    int* res = calloc(graph->size, sizeof(int));
    if (!res) {
        free(flags);
        return 0;
    }
    stack_t* stack = listCreate();
    if (!stack) {
        free(flags);
        return 0;
    }

    int cur, depth;
    for (int i = 0; i < graph->size; i++) {
        cur = i;
        depth = 0;
        do {
            // unable to think of a way to unwind stack so values will be stored immediately
            res[depth] = cur;
            int size = graph->links[i]->size;
            for (int i = 0; i < size; i++) {
                // i did not want to change list to store two ints or use bit shifting
                union {
                    int num;
                    struct {
                        uint16_t node;
                        uint16_t depth;
                    } data;
                } val;
            }
        } while (stack->size > 0);
    }
    free(flags);
    listDelete(stack);
    return 1;
}

int hamiltonRecursiveWrap(graph_t* graph, FILE* out) {
    bool* flags = calloc(graph->size, sizeof(bool));
    if (!flags) {
        return 0;
    }
    int* res = NULL;
    for (int i = 0; i < graph->size; i++) {
        // start from each node, important if path is not a cycle
        res = hamiltonPathRecursive(graph, 0, i, flags);
        if (res) {
            break;
        }
    }
    if (res) {
        for (int i = 0; i < graph->size - 1; i++) {
            fprintf(out, "%d ", res[i] + 1);
        }
        fprintf(out, "%d\n", res[graph->size - 1] + 1);
    } else {
        return 0;
    }
    return 1;
}

int* hamiltonPathRecursive(graph_t* graph, int depth, int index, bool* flags) {
    if (depth < graph->size - 1) {
        flags[index] = 1;
        node_t* node = graph->links[index]->pHead;
        int size = graph->links[index]->size;
        for (int i = 0; i < size; i++) {
            int val = node->data;
            node = node->pNext;
            int* ptr = NULL;
            // if node is unvisited
            if (!flags[val]) {
                if ((ptr = hamiltonPathRecursive(graph, depth + 1, val, flags))) {
                    ptr[depth] = index;
                    return ptr;
                }
            }
        }
        // if no solution was found
        flags[index] = 0;
        return NULL;
    } else {
        int* res = malloc(graph->size * sizeof(int));
        res[depth] = index;
        return res;
    }
}
