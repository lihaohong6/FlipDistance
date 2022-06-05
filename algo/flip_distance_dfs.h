//
// Created by Peter Li on 6/3/22.
//

#ifndef FLIPDISTANCE_FLIP_DISTANCE_DFS_H
#define FLIPDISTANCE_FLIP_DISTANCE_DFS_H

#include "flip_distance.h"
#include <algorithm>

class FlipDistanceDfs : public FlipDistance {
private:
    bool flips[100][100];
public:
    FlipDistanceDfs(TriangulatedGraph start, TriangulatedGraph end)
            : FlipDistance(std::move(start), std::move(end)) {}

    bool search(TriangulatedGraph &g, TriangulatedGraph &g2, size_t remainingFlips) {
        if (g == g2) {
            return true;
        }
        if (remainingFlips == 0) {
            return false;
        }
        for (Edge e: g2.getEdges()) {
            if (g.hasEdge(e)) {
                continue;
            }
            Edge result = g2.flip(e);
            if (g.hasEdge(result)) {
                bool res = search(g, g2, remainingFlips - 1);
                g2.flip(result);
                return res;
            }
            g2.flip(result);
        }
        std::vector<Edge> edges;
        for (Edge e: g.getEdges()) {
            if (g2.hasEdge(e)) {
                continue;
            }
            Edge result = g.flip(e);
            g.flip(result);
            if (flips[result.first][result.second]) {
                continue;
            }
            edges.emplace_back(e);
            if (g2.hasEdge(result)) {
                edges.clear();
                edges.push_back(e);
                break;
            }
        }
        if (edges.size() > remainingFlips) {
            return false;
        }
        for (Edge e: edges) {
            Edge result = g.flip(e);
            flips[e.first][e.second] = true;
            bool res = search(g, g2, remainingFlips - 1);
            flips[e.first][e.second] = false;
            g.flip(result);
            if (res) {
                return res;
            }
        }
        return false;
    }

    bool flipDistanceDecision(unsigned int k) override {
        std::fill(flips[0], flips[0] + 100 * 100, 0);
        TriangulatedGraph s = start, e = end;
        return search(s, e, k);
    }
};

#endif //FLIPDISTANCE_FLIP_DISTANCE_DFS_H
