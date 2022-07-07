#include <iostream>
#include "triangulation/TriangulatedGraph.h"
#include "algo/flip_distance_bfs.h"
#include "algo/flip_distance_source.h"
#include "algo/flip_distance_source_fast.h"
#include "triangulation/Helper.h"
#include "algo/statistics.h"
#include <unordered_map>
#include <ctime>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <algorithm>

using namespace std;

// placeholder in case this is not enough
typedef int64_t BigInt;

BigInt pow(int base, int exp) {
    BigInt ans = 1;
    while (exp--) {
        ans *= base;
    }
    return ans;
}

void printTriangulation(const TriangulatedGraph &g) {
    printf("%lu\n", g.getSize());
    for (Edge e: g.getEdges()) {
        printf("%d %d\n", e.first, e.second);
    }
}

FlipDistance *getAlgoByName(const std::string &name, TriangulatedGraph &g, TriangulatedGraph &g2) {
    if (name == "bfs") return new FlipDistanceBfs(g, g2);
    if (name == "source") return new FlipDistanceSource(g, g2);
    if (name == "source-fast") return new FlipDistanceSourceFast(g, g2);
    printf("No algorithm named %s found.", name.c_str());
    exit(1);
}

template<class T>
std::string vectorToString(const std::vector<T> &v) {
    std::string res;
    for (auto i: v) {
        res += std::to_string(i) + " ";
    }
    return res;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Need at least 2 arguments.");
        return 1;
    }
    if (strcmp(argv[1], "-c") == 0) {
        BinaryString bs(treeStringToParentheses(argv[2]));
        printTriangulation(TriangulatedGraph(bs.getBits()));
        printf("%s\n", bs.toString().c_str());
        return 0;
    }
    std::string
            s1 = std::string(argv[1]),
            s2 = std::string(argv[2]);

    TriangulatedGraph g(BinaryString(treeStringToParentheses(s1)).getBits());
    TriangulatedGraph g2(BinaryString(treeStringToParentheses(s2)).getBits());
    std::string name = argc > 3 ? argv[3] : "bfs";
    FlipDistance *m = getAlgoByName(name, g, g2);
    bool decision = false;
    if (argc > 4) {
        int input;
        sscanf(argv[4], "%d", &input);
        decision = input;
    }
    if (decision) {
        size_t start = 1, end = g.getSize() * 2 - 6;
        printf("k | result | time(s) | # branches | 1.618^n 9^(k-n) | 4^k\n");
        for (size_t i = start; i <= end; ++i) {
            resetStatistics();
            clock_t startTime = clock();
            printf("%zu %d ", i, m->flipDistanceDecision(i));
            clock_t endTime = clock();
            auto statistics = m->getStatistics();
            auto n = g.getSize() - 3;
            statistics.push_back((long long)(
                    pow(1.618L, n) *
                    pow(9.0L, std::max(0, (int) (i - n)))));
            statistics.push_back(pow(4, (int)i));
            printf("%.3f %s\n",
                   (double) (endTime - startTime) / CLOCKS_PER_SEC,
                   vectorToString(statistics).c_str());
        }
    } else {
        clock_t startTime = clock();
        printf("%d\n", m->flipDistance());
        clock_t endTime = clock();
        printf("%.2f\n", (double) (endTime - startTime) / CLOCKS_PER_SEC);
        printf("0\n");
    }
    delete m;
    return 0;
}
