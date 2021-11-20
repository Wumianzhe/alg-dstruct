#include "graph.h"
#include "queue.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void graphDelete(graph_t* graph) {
    if (graph) {
        for (int i = 0; i < graph->size; i++) {
            queueDelete(graph->links[i]);
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
        graph->links = malloc(size * sizeof(queue_t));
        if (!graph->links) {
            free(graph);
            return NULL;
        }
        for (int i = 0; i < size; i++) {
            graph->links[i] = queueCreate();
            if (!graph->links[i]) {
                for (int k = 0; k < i; k++) {
                    queueDelete(graph->links[k]);
                }
                free(graph);
                return NULL;
            }
        }
        while (fscanf(in, "%d %d\n", &first, &second) > 0) {
            enqueue(graph->links[first], second);
            enqueue(graph->links[second], first);
            if (errno) {
                graphDelete(graph);
                return NULL;
            }
        }
    }
    return graph;
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
    int cur = 0;
    flags[0] = 1;
    do {
        fprintf(out, "%d", cur);
        int size = graph->links[cur]->size;
        for (int i = 0; i < size; i++) {
            int n = dequeue(graph->links[cur]);
            if (!flags[n]) {
                flags[n] = 1;
                enqueue(queue, n);
                if (errno) {
                    queueDelete(queue);
                    return 0;
                }
            }
        }
        if (queue->size > 0) {
            fprintf(out, " ");
        }
        // empty queue returns 0 when attempting to dequeue
    } while ((cur = dequeue(queue)));
    free(flags);
    queueDelete(queue);
    return 1;
}
