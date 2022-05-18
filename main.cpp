#include <iostream>
#include "triangulation/TriangulatedGraph.h"
#include "algo/flip_distance_middle.h"

using namespace std;

int main(int argc, char **argv) {
    
    if (argc != 3) {
        printf("Need two arguments.");
        return 0;
    }
    std::string 
        s1 = std::string(argv[1]),
        s2 = std::string(argv[2]);
    
    TriangulatedGraph g(BinaryString(s1).getBits());
    TriangulatedGraph g2(BinaryString(s2).getBits());
    FlipDistanceMiddle m(g, g2);
    printf("%d\n", m.flipDistance());
    return 0;
}
