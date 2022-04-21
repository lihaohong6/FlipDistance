#include <iostream>
#include "triangulation/TriangulatedGraph.h"
#include "algo/flip_distance.h"

using namespace std;

int main() {
    TriangulatedGraph g(5);
    FlipDistance fd(g, g);
    return 0;
}
