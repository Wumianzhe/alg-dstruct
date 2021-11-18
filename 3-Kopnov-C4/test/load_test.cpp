#include "graph.h"
#include <cstdio>
#include <gtest/gtest.h>

using namespace std;

/*
 * OS: Arch Linux x86_64
 * PC configuration:
 *      CPU: AMD Ryzen 5 4500U (6) @ 2.375 GHz
 *      RAM: 16 GB
 *      SSD: 512 GB nvme
 */

/*
 * load test results (cmake build type release):
 * Time: 2.992s to generate graph file
 *       31.01s to read and walk around the graph
 * RAM:  up to 1.344 GiB
 */

const char* filename = "loadTestGraph";
const int verticesCount = (int)5e6;

TEST(loadTest, graphGeneration) {
    // link cont is ~4.5*verticesCount to make most nodes connected
    const int maxNeighbourDelta = verticesCount / 5;
    FILE* graphFile = fopen(filename, "w");
    if (!graphFile) {
        printf("Error while opening file\n");
        FAIL();
    }
    fprintf(graphFile, "%d\n", verticesCount);
    for (int i = 0; i < verticesCount; i++) {
        for (int neighbour = i + rand() % (maxNeighbourDelta - 1) + 1; neighbour < verticesCount;
             neighbour += rand() % (maxNeighbourDelta - 1) + 1) {
            fprintf(graphFile, "%d %d\n", i, neighbour);
        }
    }
    fclose(graphFile);
}

TEST(loadTest, graphReadAndBFS) {
    const char* filenameOut = "/dev/null";
    FILE* graphFile = fopen(filename, "r");
    if (!graphFile) {
        perror("Error while opening file");
        FAIL();
    }
    graph_t* graph = graphRead(graphFile);
    fclose(graphFile);
    if (!graph) {
        perror("Error during graph creation");
        FAIL();
    }

    FILE* outFile = fopen(filenameOut, "w");
    if (!outFile) {
        graphDelete(graph);
        perror("Error while opening file");
        FAIL();
    }
    breadthFirstSearch(graph, outFile);
    graphDelete(graph);
    fclose(outFile);
}
