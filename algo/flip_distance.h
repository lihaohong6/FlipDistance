//
// Created by Peter Li on 4/17/22.
//

#ifndef FLIPDISTANCE_FLIP_DISTANCE_H
#define FLIPDISTANCE_FLIP_DISTANCE_H

#include <utility>

#include "../triangulation/TriangulatedGraph.h"

struct Action {
    const int type;
    const Edge edge;

    Action(int type, Edge edgeStart) : type(type), edge(std::move(edgeStart)) {}
};

class FlipDistance {
private:
    const TriangulatedGraph start, end;
    unsigned int flips = 0;
    std::vector<Edge> oLex;

public:
    FlipDistance(TriangulatedGraph start, TriangulatedGraph end) : start(std::move(start)), end(std::move(end)) {}

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
        for (Edge o: ordering) {
            std::vector<Action> actions;
            std::function<bool(TriangulatedGraph&, int, Edge)> actionsRecursive =
                    [&](TriangulatedGraph &cur, int action, Edge edge) {
                        if (action == 2 * partition[i] - 1) {
                            return flipDistanceSearch(cur, i + 1, partition);
                        }
                        bool res = false;
                        std::vector<Edge> neighbors = cur.getNeighbors(edge);
                        for (Edge e: neighbors) {
                            if (cur.flippable(e)) {
                                actions.emplace_back(0, edge);
                                res = res || actionsRecursive(cur, action + 1, e);
                                actions.pop_back();
                            }
                        }
                        if (!res) {
                            Edge result = cur.flip(edge);
                            actions.emplace_back(1, edge);
                            res = actionsRecursive(cur, action + 1, actions.back().edge);
                            actions.pop_back();
                            cur.flip(result);
                        }
                        return res;
                    };
            if (actionsRecursive(cur, 0, o)) {
                return true;
            }
        }
        return false;
    }

    bool flipDistanceDecision(unsigned int k) {
        this->flips = k;
        std::vector<std::vector<unsigned int>> partitions;
        std::vector<unsigned int> cur;
        findPartitions(k, k, partitions, cur);
//        oLex.clear();
//        for (const Node &v: start.vertices) {
//            for (int neighbor: v.neighbors) {
//                if (v.id > neighbor) {
//                    break;
//                }
//                oLex.emplace_back(v.id, neighbor);
//            }
//        }
//        std::reverse(oLex.begin(), oLex.end());
        for (const auto &p : partitions) {
            TriangulatedGraph g = start;
            if (flipDistanceSearch(g, 0, p)) {
                return true;
            }
        }
        return false;
    }

    unsigned int flipDistance() {
        if (start == end) {
            return 0;
        }
        size_t min = 1, max = start.getSize() * 2 - 6;
        while (min < max) {
            size_t mid = (min + max) / 2;
            if (flipDistanceDecision(mid)) {
                max = mid;
            } else {
                min = mid + 1;
            }
        }
        return min;
    }
};

#endif //FLIPDISTANCE_FLIP_DISTANCE_H
