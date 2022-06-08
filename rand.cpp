//
// Created by Peter Li on 5/20/22.
//
#include <iostream>
#include <cstdio>
#include "triangulation/TriangulatedGraph.h"
#include <vector>
#include <random>
#include "triangulation/Helper.h"

bool randBool() {
    static std::random_device rd;
    static std::mt19937 mt(rd());
    static std::uniform_int_distribution<int> dist(0, 1);
    return (bool) dist(mt);
}

inline std::vector<bool> randBits(int n) {
    std::vector<bool> result;
    int totalTrue = 0;
    int currTrue = 0;
    for (int i = 0; i < 2 * n; i++) {
        bool bit;
        if (currTrue == 0) {
            bit = true;
        } else if (totalTrue == n) {
            bit = false;
        } else {
            bit = randBool();
        }
        result.push_back(bit);
        totalTrue += bit;
        currTrue += bit * 2 - 1;
    }
    return result;
}

bool simpleLeftToRight(TriangulatedGraph &s, const TriangulatedGraph &t) {
    for (Edge e: s.getEdges()) {
        Edge result = s.flip(e);
        s.flip(result);
        if (t.hasEdge(e) || t.hasEdge(result)) {
            return true;
        }
    }
    return false;
}

bool isSimple(TriangulatedGraph &s, TriangulatedGraph &t) {
    return simpleLeftToRight(s, t) || simpleLeftToRight(t, s);
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
    while (true) {
        TriangulatedGraph s(randBits(n)), t(randBits(n));
        if (!noSimple || !isSimple(s, t)) {
            std::cout
                    << binaryStringToTreeRep(s.toVector())
                    << "\n"
                    << binaryStringToTreeRep(t.toVector())
                    << "\n";
            s.getSources();
            break;
        }
    }
}