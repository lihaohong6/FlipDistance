//
// Created by Peter Li on 4/23/22.
//

#ifndef FLIPDISTANCE_FLIP_DISTANCE_FAST_H
#define FLIPDISTANCE_FLIP_DISTANCE_FAST_H


#include <utility>

#include "../triangulation/TriangulatedGraph.h"
#include "flip_distance.h"

class FlipDistanceFast : public FlipDistance {
private:
    std::vector<Edge> oLex;
    unsigned int flips = 0;

public:
    FlipDistanceFast(TriangulatedGraph start, TriangulatedGraph end)
            : FlipDistance(std::move(start), std::move(end)) {}

    void findPartitions(unsigned int rem, unsigned int max, std::vector<std::vector<unsigned int>> &result,
                        std::vector<unsigned int> &cur) {
        if (rem == 0) {
            result.push_back(cur);
        }
        for (int i = 1; i <= std::min(max, rem); i++) {
            cur.push_back(i);
            findPartitions(rem - i, i, result, cur);
            cur.pop_back();
        }
    }

    static TriangulatedGraph transform(TriangulatedGraph start, const std::vector<Action> &actions) {
        for (auto action: actions) {
            if (action.type == 1) {
                auto e = action.edge;
                start.flip(e.first, e.second);
            }
        }
        return start;
    }

    bool flipDistanceSearch(TriangulatedGraph &cur, int i, const std::vector<unsigned int> &partition) {
        if (i == partition.size()) {
            return cur == end;
        }
        std::vector<Edge> ordering;
        for (const auto &v: cur.vertices) {
            for (int v2: v.neighbors) {
                if (v.id < v2 && !end.hasEdge(v.id, v2)) {
                    ordering.emplace_back(v.id, v2);
                }
            }
        }
        unsigned int remaining = 0;
        for (int c = i; c < partition.size(); c++) {
            remaining += partition[c];
        }
        if (ordering.size() > remaining) {
            return false;
        }
        for (Edge o: ordering) {
            std::vector<Action> actions;
            std::function<bool(TriangulatedGraph &, int, Edge)> actionsRecursive =
                    [&](TriangulatedGraph &cur, int action, Edge edge) {
                        if (action == 2 * partition[i] - 1) {
                            return flipDistanceSearch(cur, i + 1, partition);
                        }
                        // try to flip
                        Edge result = cur.flip(edge);
                        actions.emplace_back(1, edge);
                        bool res = actionsRecursive(cur, action + 1, actions.back().edge);
                        actions.pop_back();
                        cur.flip(result);
                        if (res || end.hasEdge(result.first, result.second)) {
                            return res;
                        }
                        // try to move
                        std::vector<Edge> neighbors = cur.getNeighbors(edge);
                        for (Edge e: neighbors) {
                            if (cur.flippable(e)) {
                                actions.emplace_back(0, edge);
                                res = res || actionsRecursive(cur, action + 1, e);
                                actions.pop_back();
                            }
                        }
                        return res;
                    };
            if (actionsRecursive(cur, 0, o)) {
                return true;
            }
        }
        return false;
    }

    bool flipDistanceDecision(unsigned int k) override {
        this->flips = k;
        std::vector<std::vector<unsigned int>> partitions;
        std::vector<unsigned int> cur;
        findPartitions(k, k, partitions, cur);
        for (const auto &p: partitions) {
            TriangulatedGraph g = start;
            if (flipDistanceSearch(g, 0, p)) {
                return true;
            }
        }
        return false;
    }

};

#endif //FLIPDISTANCE_FLIP_DISTANCE_FAST_H
