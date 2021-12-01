#include "graph.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    graph_t* graph = graphRead(stdin);
    if (!graph) {
        perror("Input error");
        return 1;
    }
    if (!hamiltonPath(graph, stdout)) {
        if (errno) {
            graphDelete(graph);
            perror("Error");
            return 1;
        }
        printf("0\n");
    }
    graphDelete(graph);
    return 0;
}
