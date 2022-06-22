//
// Created by Peter Li on 5/20/22.
//
#include <iostream>
#include <cstdio>
#include "utils/rand.h"
#include "triangulation/Helper.h"

void printRandomTriangulation(int n, bool noSimple) {
    auto p = randomTriangulation(n, noSimple);
    std::cout
            << binaryStringToTreeRep(p.first.toVector())
            << "\n"
            << binaryStringToTreeRep(p.second.toVector())
            << "\n";
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Expect at least one command line argument.");
        return 1;
    }
    int n;
    sscanf(argv[1], "%d", &n);
    bool noSimple = true;
    if (argc == 3) {
        int t;
        sscanf(argv[2], "%d", &t);
        noSimple = t;
    }
    printRandomTriangulation(n, noSimple);
}