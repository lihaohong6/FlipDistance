//
// Created by Peter Li on 6/8/22.
//

#ifndef FLIPDISTANCE_RAND_H
#define FLIPDISTANCE_RAND_H

#include <utility>
#include "../triangulation/TriangulatedGraph.h"

std::pair<TriangulatedGraph, TriangulatedGraph> randomTriangulation(int n, bool noSimple = true);

#endif //FLIPDISTANCE_RAND_H
