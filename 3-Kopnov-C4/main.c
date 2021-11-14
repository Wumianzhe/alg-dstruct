#include "inc/graph.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    int size;
    FILE* in = fopen("template", "r");
    if (!in) {
        perror("Error opening file");
        return -1;
    }
    fscanf(in, "%d\n", &size);
    graph_t* graph = graphCreate(size);
    if (!graph) {
        fclose(in);
        perror("");
        return -1;
    }
    graphRead(graph, in);
    breadthFirstSearch(graph, stdout);
    graphDelete(graph);
    return 0;
}
