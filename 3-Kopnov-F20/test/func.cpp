#include "tree.h"
#include <cstdio>
#include <gtest/gtest.h>

bool fileCmp(FILE* in, FILE* expected) {
    fseek(in, 0, SEEK_END);
    fseek(expected, 0, SEEK_END);
    if (ftell(in) != ftell(expected)) {
        return false;
    }
    int size = ftell(in);
    fseek(in, 0, SEEK_SET);
    fseek(expected, 0, SEEK_SET);

    // byte-comparison
    char bufIn[256];
    char bufExp[256];
    for (int i = 0; size / 256; i++) {
        fread(bufIn, 1, 256, in);
        fread(bufExp, 1, 256, expected);
        if (memcmp(bufIn, bufExp, 256)) {
            return false;
        }
    }
    fread(bufIn, 1, size % 256, in);
    fread(bufExp, 1, size % 256, expected);
    if (memcmp(bufIn, bufExp, size % 256)) {
        return false;
    }
    return true;
}

void testPath(const char* filenameIn, const char* filenameOut, const char* filenameExpected) {
    FILE* in = fopen(filenameIn, "r");
    if (!in) {
        perror("");
        FAIL();
    }
    FILE* out = fopen(filenameOut, "w");
    if (!out) {
        perror("");
        fclose(in);
        FAIL();
    }
    int k;
    fscanf(in, "%d\n", &k);
    wrapMain(in, out, k);
    fclose(in);
    fclose(out);
    in = fopen(filenameOut, "r");
    if (!in) {
        perror("");
        FAIL();
    }
    FILE* expect = fopen(filenameExpected, "r");
    if (!expect) {
        perror("");
        fclose(in);
        FAIL();
    }
    EXPECT_TRUE(fileCmp(in, expect));
    fclose(in);
    fclose(expect);
}

#define INPUT "test/input/"
#define EXPECTED "test/expected/"

TEST(func, smallTree) { testPath(INPUT "small", "out.txt", EXPECTED "small"); }
TEST(func, odd) { testPath(INPUT "odd", "out.txt", EXPECTED "odd"); }
TEST(func, even) { testPath(INPUT "even", "out.txt", EXPECTED "even"); }
