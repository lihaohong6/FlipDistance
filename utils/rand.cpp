//
// Created by Peter Li on 6/8/22.
//

#include "rand.h"
#include <vector>
#include <random>

bool randBool() {
    static std::random_device rd;
    static std::mt19937 mt(rd());
    static std::uniform_int_distribution<int> dist(0, 1);
    return (bool) dist(mt);
}

std::vector<bool> randBits(int n) {
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

std::pair<TriangulatedGraph, TriangulatedGraph> randomTriangulation(int n, bool noSimple) {
    while (true) {
        TriangulatedGraph s(randBits(n - 3)), t(randBits(n - 3));
        if (!noSimple || !isSimple(s, t)) {
            return {s, t};
        }
    }
}
