//
// Created by Peter Li on 4/26/22.
//

#ifndef FLIPDISTANCE_FLIP_DISTANCE_SIMPLE_H
#define FLIPDISTANCE_FLIP_DISTANCE_SIMPLE_H

#include "flip_distance.h"
#include "../triangulation/TriangulatedGraph.h"
#include <algorithm>

class FlipDistanceSimple : public FlipDistance {
private:
    bool flips[100][100];
    std::vector<std::string> visited;
public:
    FlipDistanceSimple(TriangulatedGraph start, TriangulatedGraph end)
            : FlipDistance(std::move(start), std::move(end)) {}

    bool search(TriangulatedGraph &g, size_t remainingFlips) {
        visited.push_back(g.toBinaryString().toString());
        if (g == end) {
            return true;
        }
        if (remainingFlips == 0) {
            return false;
        }
        for (Edge e: g.getEdges()) {
            if (end.hasEdge(e)) {
                continue;
            }
            Edge result = g.flip(e);
            if (flips[result.first][result.second]) {
                g.flip(result);
                continue;
            }
            flips[e.first][e.second] = true;
            bool res = search(g, remainingFlips - 1);
            flips[e.first][e.second] = false;
            g.flip(result);
            if (res) { //  || end.hasEdge(result)
                return res;
            }
        }
        return false;
    }

    const std::vector<std::string> &getVisited() const {
        return visited;
    }

    bool flipDistanceDecision(unsigned int k) override {
        size_t n = start.getSize();
        std::fill(flips[0], flips[0] + n * n, 0);
        TriangulatedGraph s = start;
        return search(s, k);
    }
};

#endif //FLIPDISTANCE_FLIP_DISTANCE_SIMPLE_H
