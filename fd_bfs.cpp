#include <iostream>
#include "triangulation/TriangulatedGraph.h"
#include "algo/flip_distance_middle.h"
#include "algo/flip_distance_bfs.h"
#include "triangulation/Helper.h"
#include <ctime>
#include <cstring>

using namespace std;

void printTriangulation(const TriangulatedGraph &g) {
    printf("%lu\n", g.getSize());
    for (Edge e : g.getEdges()) {
        printf("%d %d\n", e.first, e.second);
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Need 2 arguments.");
        return 1;
    }
    std::string
            s1 = std::string(argv[1]),
            s2 = std::string(argv[2]);

    TriangulatedGraph g(BinaryString(treeStringToParentheses(s1)).getBits());
    TriangulatedGraph g2(BinaryString(treeStringToParentheses(s2)).getBits());
    FlipDistanceBfs m(g, g2);
    clock_t start = clock();
    printf("%d\n", m.flipDistance());
    clock_t end = clock();
    printf("%.2f\n", (double)(end - start) / CLOCKS_PER_SEC);
    printf("%zu", m.hashSetSize);
    return 0;
}
