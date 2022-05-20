#include <iostream>
#include "triangulation/TriangulatedGraph.h"
#include "algo/flip_distance_middle.h"
#include "algo/flip_distance_bfs.h"
#include "triangulation/Helper.h"
#include <ctime>

using namespace std;

int main(int argc, char **argv) {
    
    if (argc != 3) {
        printf("Need two arguments.");
        return 0;
    }
    std::string 
        s1 = std::string(argv[1]),
        s2 = std::string(argv[2]);
    
    TriangulatedGraph g(BinaryString(treeStringToParentheses(s1)).getBits());
    TriangulatedGraph g2(BinaryString(treeStringToParentheses(s2)).getBits());
    FlipDistanceMiddle m(g, g2);
    clock_t start = clock();
    printf("%d\n", m.flipDistance());
    clock_t end = clock();
    printf("%.2f", (double)(end - start) / CLOCKS_PER_SEC);
    return 0;
}
