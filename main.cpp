#include <iostream>
#include "triangulation/TriangulatedGraph.h"
#include "algo/flip_distance_simple.h"

using namespace std;

int main() {
    TriangulatedGraph g(10);
    g.addEdge(0, 3);
    g.addEdge(0, 4);
    g.addEdge(1, 3);
    g.addEdge(4, 9);
    g.addEdge(5, 7);
    g.addEdge(5, 9);
    g.addEdge(9, 7);
    TriangulatedGraph g2(10);
    g2.addEdge(1, 6);
    g2.addEdge(1, 8);
    g2.addEdge(1, 9);
    g2.addEdge(2, 4);
    g2.addEdge(2, 5);
    g2.addEdge(2, 6);
    g2.addEdge(6, 8);
    FlipDistanceSimple fd(g, g2);
    printf("%d\n", fd.flipDistanceDecision(8));
    printf("%lu\n", fd.getVisited().size());
    return 0;
}
