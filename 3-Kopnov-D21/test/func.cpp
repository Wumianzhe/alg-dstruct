#include "graph.h"
#include <errno.h>
#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>

void testPath(const char* filenameIn, const char* filenameOut, const char* filenameExpected) {
    FILE* in = fopen(filenameIn, "r");
    if (!in) {
        perror("Input error");
        FAIL();
    }
    graph_t* graph = graphRead(in);
    fclose(in);
    if (!graph) {
        perror("Input error");
        FAIL();
    }
    FILE* out = fopen(filenameOut, "w");
    if (!out) {
        perror("Input error");
        graphDelete(graph);
        FAIL();
    }
    if (!hamiltonPath(graph, out)) {
        if (errno) {
            graphDelete(graph);
            perror("Error");
            FAIL();
        }
        fprintf(out, "0\n");
    }
    fclose(out);
    graphDelete(graph);

    out = fopen(filenameOut, "r");
    if (!out) {
        perror("Input error");
        FAIL();
    }
    FILE* expected = fopen(filenameExpected, "r");
    if (!out) {
        fclose(out);
        perror("Input error");
        FAIL();
    }
    int val = 0, exp = 0;
    // wheteher files are of same size
    fseek(out, 0, SEEK_END);
    fseek(expected, 0, SEEK_END);
    if (ftell(out) != ftell(expected)) {
        fclose(out);
        fclose(expected);
        FAIL();
    }
    fseek(out, 0, SEEK_SET);
    fseek(expected, 0, SEEK_SET);

    // since format is set, size equality means equal number count (either 1 or all),
    // so it's possible to compare one by one
    while ((fscanf(out, "%d ", &val) > 0) && (fscanf(expected, "%d ", &exp) > 0)) {
        if (val != exp) {
            fclose(out);
            fclose(expected);
            FAIL();
        }
    }
}

TEST(func, pdfExample) { testPath("test/inputs/pdf", "out.txt", "test/expect/pdf"); }

TEST(func, noPath) { testPath("test/inputs/noPath", "out.txt", "test/expect/noPath"); }

TEST(func, pathFromOne) { testPath("test/inputs/yesPath", "out.txt", "test/expect/yesPath"); }

TEST(func, pathFromN) { testPath("test/inputs/pathFromN", "out.txt", "test/expect/pathFromN"); }

TEST(func, AcyclicPath) { testPath("test/inputs/acyclic", "out.txt", "test/expect/acyclic"); }
