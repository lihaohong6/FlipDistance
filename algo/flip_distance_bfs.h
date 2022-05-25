//
// Created by Peter Li on 5/19/2022.
//

#ifndef FLIPDISTANCE_FLIP_DISTANCE_BFS_H
#define FLIPDISTANCE_FLIP_DISTANCE_BFS_H

#include <queue>
#include <vector>
#include <unordered_set>
#include "flip_distance.h"
#include "../triangulation/BinaryString.h"

class FlipDistanceBfs : public FlipDistance {
public:
    size_t hashSetSize = 0;
    FlipDistanceBfs(TriangulatedGraph start, TriangulatedGraph end)
            : FlipDistance(std::move(start), std::move(end)) {}

    unsigned int flipDistance() override {
        std::queue<std::vector<bool>> bfs;
        std::vector<bool>
                startBits = start.toBinaryString().getBits();
        bfs.push(startBits);
        std::unordered_set<std::vector<bool>> set;
        set.insert(startBits);
        for (int dist = 1; dist <= 2 * start.getSize() - 6; ++dist) {
            std::queue<std::vector<bool>> nextQueue;
            while (!bfs.empty()) {
                std::vector<bool> v(bfs.front());
                bfs.pop();
                TriangulatedGraph g(v);
                std::vector<Edge> candidates;
                for (Edge e: g.getEdges()) {
                    if (end.hasEdge(e)) {
                        continue;
                    }
                    Edge result = g.flip(e);
                    g.flip(result);
                    if (end.hasEdge(result)) {
                        candidates.clear();
                        candidates.push_back(e);
                        break;
                    }
                    candidates.push_back(e);
                }
                for (Edge e: candidates) {
                    Edge result = g.flip(e);
                    if (g == end) {
                        hashSetSize = set.size();
                        return dist;
                    }
                    std::vector<bool> v2 = g.toVector();
                    g.flip(result);
                    if(set.insert(v2).second) {
                        nextQueue.push(v2);
                    }
                }
            }
            bfs = nextQueue;
        }
        fprintf(stderr, "Unexpected Error: Flip Distance not found.");
        return -1;
    }
};

#endif //FLIPDISTANCE_FLIP_DISTANCE_BFS_H
