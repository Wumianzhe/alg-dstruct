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
 * Time: 0.007s to generate graph file (size 8000)
 *       35.64 to read and walk around the graph
 * RAM:  up to 2.99 MiB
 *
 *  Recursion: highest record is 80 vertices in 20.168 seconds, no memory data
 */

const char* filename = "loadTestGraph";
const int verticesCount = 8000;

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
            fprintf(graphFile, "%d %d\n", i + 1, neighbour + 1);
        }
    }
    fclose(graphFile);
}

TEST(loadTest, graphPath) {
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
    printf("read done\n");

    FILE* outFile = fopen(filenameOut, "w");
    if (!outFile) {
        graphDelete(graph);
        perror("Error while opening file");
        FAIL();
    }
    hamiltonPath(graph, outFile);
    graphDelete(graph);
    fclose(outFile);
}

// TEST(loadTest, graphPathRecursive) {
//     const char* filenameOut = "/dev/null";
//     FILE* graphFile = fopen(filename, "r");
//     if (!graphFile) {
//         perror("Error while opening file");
//         FAIL();
//     }
//     graph_t* graph = graphRead(graphFile);
//     fclose(graphFile);
//     if (!graph) {
//         perror("Error during graph creation");
//         FAIL();
//     }
//     printf("read done\n");

//     FILE* outFile = fopen(filenameOut, "w");
//     if (!outFile) {
//         graphDelete(graph);
//         perror("Error while opening file");
//         FAIL();
//     }
//     hamiltonRecursiveWrap(graph, outFile);
//     graphDelete(graph);
//     fclose(outFile);
// }
