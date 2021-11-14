#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct node {
    struct node* pNext;
    int data;
} node_t;

typedef struct {
    node_t* pHead;
    node_t* pTail;
    int size;
} queue_t;

void enqueue(queue_t* queue, int n);
int dequeue(queue_t* queue);
node_t* nodeCreate(int n);
queue_t* queueCreate();
void queueDelete(queue_t* queue);

typedef struct {
    int size;
    queue_t** links; // array of any lists can be used and queues are already defined
} graph_t;

graph_t* graphCreate(int size);
void graphDelete(graph_t* graph);
void graphRead(graph_t* graph, FILE* in);
int breadthFirstSearch(graph_t* graph, FILE* out);

int main(int argc, char* argv[]) {
    int size;
    fscanf(stdin, "%d\n", &size);
    graph_t* graph = graphCreate(size);
    if (!graph) {
        perror("");
        return -1;
    }
    graphRead(graph, stdin);
    breadthFirstSearch(graph, stdout);
    graphDelete(graph);
    return 0;
}

void enqueue(queue_t* queue, int n) {
    node_t* node = nodeCreate(n);
    if (!node) {
        perror("Enqueue malloc error");
        return;
    }
    if (queue->size > 0) {
        queue->pTail->pNext = node;
        queue->pTail = node;
    } else {
        queue->pHead = node;
        queue->pTail = node;
    }
    queue->size++;
}
int dequeue(queue_t* queue) {
    if (queue->size > 0) {
        node_t* node = queue->pHead;
        int val = node->data;
        queue->pHead = node->pNext;
        queue->size--;
        free(node);
        return val;
    } else {
        errno = EINVAL;
        return 0;
    }
}
node_t* nodeCreate(int n) {
    node_t* node = malloc(sizeof(node_t));
    if (node) {
        node->data = n;
        node->pNext = NULL;
    }
    return node;
}
queue_t* queueCreate() {
    queue_t* queue = malloc(sizeof(queue_t));
    if (queue) {
        queue->pHead = NULL;
        queue->pTail = NULL;
        queue->size = 0;
    }
    return queue;
}
void queueDelete(queue_t* queue) {
    node_t* node = queue->pHead;
    for (int i = 0; i < queue->size; i++) {
        node_t* tmp = node;
        node = node->pNext;
        free(tmp);
    }
    free(queue);
}

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
