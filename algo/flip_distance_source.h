//
// Created by Peter Li on 6/4/22.
//

#ifndef FLIPDISTANCE_FLIP_DISTANCE_SOURCE_H
#define FLIPDISTANCE_FLIP_DISTANCE_SOURCE_H

#include "flip_distance.h"
#include <algorithm>
#include <queue>
#include <vector>
#include <unordered_set>

struct HashEdge {
    size_t operator()(const Edge &e) const {
        return std::hash<int>()(e.first) * 31 + std::hash<int>()(e.second);
    }
};

class FlipDistanceSource : public FlipDistance {
private:
    int branchCounter = 0;

public:

    FlipDistanceSource(TriangulatedGraph start, TriangulatedGraph end)
            : FlipDistance(std::move(start), std::move(end)) {}

    static void addNeighborsToForbid(const Edge &e, const TriangulatedGraph &g,
                                     std::unordered_multiset<Edge, HashEdge> &forbid) {
        forbid.insert(e);
        for (const Edge &neighbor: g.getNeighbors(e)) {
            forbid.insert(neighbor);
        }
    }

    static void removeNeighborsFromForbid(const Edge &e, const TriangulatedGraph &g,
                                          std::unordered_multiset<Edge, HashEdge> &forbid) {
        forbid.erase(e);
        for (const Edge &neighbor: g.getNeighbors(e)) {
            forbid.erase(neighbor);
        }
    }

    bool splitAndSearch(const TriangulatedGraph &g, Edge &divider, 
                        int k, // keep as int; possible overflow for unsigned int
                        const std::vector<Edge> &sources) {
        if (k <= 0) {
            return g == end && k == 0;
        }
        int v1 = divider.first, v2 = divider.second;
        TriangulatedGraph s1 = g.subGraph(v1, v2), e1 = end.subGraph(v1, v2);
        auto sources1 = g.filterAndMapEdges(v1, v2, sources);
        TriangulatedGraph s2 = g.subGraph(v2, v1), e2 = end.subGraph(v2, v1);
        auto sources2 = g.filterAndMapEdges(v2, v1, sources);
        FlipDistanceSource algo(s1, e1);
        for (auto i = s1.getSize() - 3; i <= k; ++i) {
            // FIXME: use sources1 and sources2
            if (algo.flipDistanceDecision(i)) {
                FlipDistanceSource algo2(s2, e2);
                return algo2.flipDistanceDecision(k - i);
            }
        }
        return false;
    }

    bool search(const std::vector<Edge> &sources, TriangulatedGraph g, 
                int k) { // keep as int; possible overflow for unsigned int
        if (g == end && k >= 0) {
            return true;
        }
        if (g.getSize() - 3 > k) {
            return false;
        }
        if (sources.empty()) {
            return false;
        }
        for (const Edge &e: g.getEdges()) {
            Edge result = g.flip(e);
            if (end.hasEdge(result)) {
                bool ret = std::count(sources.begin(), sources.end(), e) > 0 &&
                           splitAndSearch(g, result, k - 1, sources);
                g.flip(result);
                return ret;
            }
            g.flip(result);
        }
        std::unordered_multiset<Edge, HashEdge> forbid;
        std::vector<std::pair<Edge, Edge>> next;
        for (const Edge &e: sources) {
            assert(g.flippable(e));
            Edge result = g.flip(e);
            auto neighbors = g.getNeighbors(result);
            next.emplace_back(neighbors[0], neighbors[1]);
            next.emplace_back(neighbors[2], neighbors[3]);
        }
        k -= (int) sources.size();
        std::vector<Edge> cur;
        std::function<bool(int)> generateNext = [&](int index) -> bool {
            if (index == next.size()) {
                return search(cur, g, k);
            }
            if (generateNext(index + 1)) {
                return true;
            }
            for (const Edge &e: {next[index].first, next[index].second}) {
                if (!g.flippable(e) || forbid.count(e) > 0) {
                    continue;
                }
                addNeighborsToForbid(e, g, forbid);
                cur.push_back(e);
                auto ret = generateNext(index + 1);
                removeNeighborsFromForbid(e, g, forbid);
                cur.pop_back();
                if (ret) {
                    return true;
                }
            }
            return false;
        };
        return generateNext(0);
    }

    bool flipDistanceDecision(unsigned int k, const std::vector<Edge> &source) {
        TriangulatedGraph g = start;
        return search(source, g, (int)k);
    }

    bool flipDistanceDecision(unsigned int k) override {
        TriangulatedGraph g = start;
        for (Edge e : g.getEdges()) {
            if (end.hasEdge(e)) {
                return splitAndSearch(g, e, (int)k, {});
            }
            Edge result = g.flip(e);
            if (end.hasEdge(result)) {
                bool ret = splitAndSearch(g, result, k - 1, {});
                g.flip(result);
                return ret;
            }
            g.flip(result);
        }
        std::vector<std::vector<Edge>> sources = start.getSources();
        for (const auto &source: sources) {
            if (flipDistanceDecision(k, source)) {
                return true;
            }
        }
        return false;
    }

    std::vector<int> getStatistics() override {
        return {branchCounter};
    }
};

#endif //FLIPDISTANCE_FLIP_DISTANCE_SOURCE_H
