#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    graph_t* graph = graphRead(stdin);
    if (!graph) {
        perror("Input error");
        return 1;
    }
    if (!hamiltonRecursiveWrap(graph, stdout)) {
        printf("0");
    }
    return 0;
}
