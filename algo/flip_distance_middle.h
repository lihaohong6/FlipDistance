//
// Created by Peter Li on 5/12/22.
//

#ifndef FLIPDISTANCE_FLIP_DISTANCE_MIDDLE_H
#define FLIPDISTANCE_FLIP_DISTANCE_MIDDLE_H

#include <queue>
#include <vector>
#include <unordered_set>
#include "flip_distance.h"
#include "../triangulation/BinaryString.h"

class FlipDistanceMiddle : public FlipDistance {
public:
    size_t hashSetSize = 0;
    
    FlipDistanceMiddle(TriangulatedGraph start, TriangulatedGraph end)
            : FlipDistance(std::move(start), std::move(end)) {}

    unsigned int flipDistance() override {
        std::queue<std::vector<bool>> bfsLeft, bfsRight;
        std::vector<bool>
                startBits = start.toBinaryString().getBits(),
                endBits = end.toBinaryString().getBits();
        bfsLeft.push(startBits);
        bfsRight.push(endBits);
        std::unordered_set<std::vector<bool>> left, right;
        left.insert(startBits);
        right.insert(endBits);
        for (int dist = 1; dist <= start.getSize() - 3; ++dist) {
            for (int cur = 0; cur < 2; ++cur) {
                const TriangulatedGraph *target = cur == 0 ? &end : &start;
                std::queue<std::vector<bool>>
                        *curQueue = cur == 0 ? &bfsLeft : &bfsRight,
                        *otherQueue = cur == 0 ? &bfsRight : &bfsLeft;
                std::unordered_set<std::vector<bool>>
                        *curSet = cur == 0 ? &left : &right,
                        *otherSet = cur == 0 ? &right : &left;
                std::queue<std::vector<bool>> nextQueue;
                while (!curQueue->empty()) {
                    std::vector<bool> v(curQueue->front());
                    curQueue->pop();
                    TriangulatedGraph g(v);
                    std::vector<Edge> candidates;
                    bool foundOneFlip = false;
                    for (Edge e: g.getEdges()) {
                        if (target->hasEdge(e)) {
                            continue;
                        }
                        Edge result = g.flip(e);
                        if (target->hasEdge(result)) {
                            if (!foundOneFlip) {
                                foundOneFlip = true;
                                candidates.clear();
                            }
                            candidates.push_back(e);
                        } else if (!foundOneFlip) {
                            candidates.push_back(e);
                        }
                        g.flip(result);
                    }
                    for (Edge e: candidates) {
                        Edge result = g.flip(e);
                        std::vector<bool> v2 = g.toVector();
                        g.flip(result);
                        if (otherSet->count(v2) > 0) {
                            this->hashSetSize = left.size() + right.size();
                            return dist * 2 - (1 - cur);
                        }
                        if (curSet->insert(v2).second) {
                            nextQueue.push(v2);
                        }
                    }
                }
                *curQueue = nextQueue;
            }
        }
        fprintf(stderr, "Unexpected Error: Flip Distance not found.");
        return -1;
    }
};

#endif //FLIPDISTANCE_FLIP_DISTANCE_MIDDLE_H