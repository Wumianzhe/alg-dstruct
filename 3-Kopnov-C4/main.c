#include "inc/graph.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    FILE* in = fopen("template", "r");
    if (!in) {
        perror("Error opening file");
        return -1;
    }
    graph_t* graph = graphRead(in);
    fclose(in);
    if (!graph) {
        perror("");
        return -1;
    }
    breadthFirstSearch(graph, stdout);
    graphDelete(graph);
    return 0;
}
