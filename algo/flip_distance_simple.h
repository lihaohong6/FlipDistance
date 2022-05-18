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
        std::vector<Edge> edges;
        for (Edge e: g.getEdges()) {
            if (end.hasEdge(e)) {
                continue;
            }
            Edge result = g.flip(e);
            g.flip(result);
            if (flips[result.first][result.second]) {
                continue;
            }
            edges.emplace_back(e);
            if (end.hasEdge(result)) {
                edges.clear();
                edges.push_back(e);
                break;
            }
        }
        for (Edge e: edges) {
            Edge result = g.flip(e);
            flips[e.first][e.second] = true;
            bool res = search(g, remainingFlips - 1);
            flips[e.first][e.second] = false;
            g.flip(result);
            if (res) {
                return res;
            }
        }
        return false;
    }

    const std::vector<std::string> &getVisited() const {
        return visited;
    }

    bool flipDistanceDecision(unsigned int k) override {
        std::fill(flips[0], flips[0] + 100 * 100, 0);
        TriangulatedGraph s = start;
        return search(s, k);
    }
};

#endif //FLIPDISTANCE_FLIP_DISTANCE_SIMPLE_H
