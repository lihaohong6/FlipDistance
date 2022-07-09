//
// Created by Peter Li on 6/8/22.
//

#include "utils/rand.h"
#include "algo/flip_distance_source.h"

int main() {
    while (1) {
        auto p = randomTriangulation(10, true);
        FlipDistanceSource algo(p.first,p.second);
        printf("%lu\n", algo.flipDistance());
    }
}
