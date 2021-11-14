#include "graph.h"
#include "queue.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

graph_t* graphCreate(int size) {
    graph_t* graph = malloc(sizeof(graph_t));
    if (graph) {
        graph->size = size;
        graph->links = malloc(size * sizeof(queue_t));
        if (!graph->links) {
            free(graph);
            return NULL;
        }
        for (int i = 0; i < size; i++) {
            graph->links[i] = queueCreate();
            if (!graph->links[i]) {
                free(graph);
                return NULL;
            }
        }
    }
    return graph;
}

void graphDelete(graph_t* graph) {
    if (graph) {
        for (int i = 0; i < graph->size; i++) {
            queueDelete(graph->links[i]);
        }
        free(graph->links);
        free(graph);
    }
}

void graphRead(graph_t* graph, FILE* in) {
    int first, second;
    while (fscanf(in, "%d %d\n", &first, &second) > 0) {
        enqueue(graph->links[first], second);
        enqueue(graph->links[second], first);
    }
}

int breadthFirstSearch(graph_t* graph, FILE* out) {
    bool* flags = calloc(graph->size, sizeof(bool));
    if (!flags) {
        return 0;
    }
    queue_t* queue = queueCreate();
    if (!queue) {
        free(flags);
        return 0;
    }
    for (int i = 0; i < graph->size; i++) {
        printf("%d: ", i);
        queuePrint(graph->links[i]);
    }
    int cur = 0;
    do {
        flags[cur] = 1;
        fprintf(out, "%d ", cur);
        int size = graph->links[cur]->size;
        for (int i = 0; i < size; i++) {
            int n = dequeue(graph->links[cur]);
            if (!flags[n]) {
                enqueue(queue, n);
            }
        }
        // empty queue returns 0 when attempting to dequeue
    } while ((cur = dequeue(queue)));
    fprintf(out, "\n");
    free(flags);
    queueDelete(queue);
    return 1;
}
