//
// Created by Peter Li on 6/4/22.
//

#ifndef FLIPDISTANCE_FLIP_DISTANCE_FPT_H
#define FLIPDISTANCE_FLIP_DISTANCE_FPT_H

#include "flip_distance.h"
#include <algorithm>
#include <queue>
#include <vector>
#include <unordered_set>

namespace std {
    template<>
    struct [[maybe_unused]] hash<Edge> {
        size_t operator()(const Edge &e) const {
            return std::hash<int>()(e.first) * 31 + std::hash<int>()(e.second);
        }
    };

}

int branchCounter = 0;

class FlipDistanceFPT : public FlipDistance {

public:

    FlipDistanceFPT(TriangulatedGraph start, TriangulatedGraph end)
            : FlipDistance(std::move(start), std::move(end)) {}
            
private:
            
    static void addNeighborsToForbid(const Edge &e, const TriangulatedGraph &g,
                                     std::unordered_multiset<Edge> &forbid) {
        forbid.insert(e);
        for (const Edge &neighbor: g.getNeighbors(e)) {
            forbid.insert(neighbor);
        }
    }

    template<class T>
    static inline void eraseOne(std::unordered_multiset<T> &set, const T &e) {
        auto find = set.find(e);
        if (find != set.end()) {
            set.erase(find);
        }
    }

    static void removeNeighborsFromForbid(const Edge &e, const TriangulatedGraph &g,
                                          std::unordered_multiset<Edge> &forbid) {
        eraseOne(forbid, e);
        for (const Edge &neighbor: g.getNeighbors(e)) {
            eraseOne(forbid, neighbor);
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
        FlipDistanceFPT algo(s1, e1);
        for (auto i = s1.getSize() - 3; i <= k; ++i) {
            // FIXME: use sources1 and sources2
            if (algo.flipDistanceDecision(i)) {
                FlipDistanceFPT algo2(s2, e2);
                return algo2.flipDistanceDecision(k - i);
            }
        }
        return false;
    }

    static inline void addNeighbors(std::vector<std::pair<Edge, Edge>> &next,
                                    const TriangulatedGraph &g, const Edge &e) {
        auto neighbors = g.getNeighbors(e);
        next.emplace_back(neighbors[0], neighbors[1]);
        next.emplace_back(neighbors[2], neighbors[3]);
    }

    static std::vector<std::pair<Edge, Edge>>
    filterAndMapEdgePairs(const std::vector<std::pair<Edge, Edge>>& sources,
                          const std::function<bool(int)>& filter,
                          const std::function<int(int)>& mapper) {
        std::vector<std::pair<Edge, Edge>> result;
        for (auto pair: sources) {
            if (filter(pair.first.first) && filter(pair.first.second)
                && filter(pair.second.first) && filter(pair.second.second)) {
                result.emplace_back(
                        std::pair(Edge(mapper(pair.first.first), mapper(pair.first.second)),
                                  Edge(mapper(pair.second.first), mapper(pair.second.second))
                        ));
            }
        }
        return result;
    }

    bool search(const std::vector<std::pair<Edge, Edge>> &sources, TriangulatedGraph g,
                int k) { // keep as int; possible overflow for unsigned int
        // sanity check
        for (const Edge &e : g.getEdges()) {
            assert(!end.hasEdge(e));
        }
        for (const Edge &e: g.getEdges()) {
            Edge result = g.flip(e);
            if (end.hasEdge(result)) {
                k--;
                std::vector<std::pair<Edge, Edge>> next = sources;
                next.erase(std::remove_if(next.begin(), next.end(), [=](auto pair) {
                    return pair.first == e || pair.second == e;
                }), next.end());
                addNeighbors(next, g, result);
                int v1 = result.first, v2 = result.second;
                TriangulatedGraph s1 = g.subGraph(v1, v2), e1 = end.subGraph(v1, v2);
                auto sources1 = filterAndMapEdgePairs(next,
                                                      TriangulatedGraph::getVertexFilter(v1, v2),
                                                      g.getVertexMapper(v1, v2));
                TriangulatedGraph s2 = g.subGraph(v2, v1), e2 = end.subGraph(v2, v1);
                auto sources2 = filterAndMapEdgePairs(next,
                                                      TriangulatedGraph::getVertexFilter(v2, v1),
                                                      g.getVertexMapper(v2, v1));
                g.flip(result);
                FlipDistanceFPT algo(s1, e1);
                for (auto i = s1.getSize() - 3; i <= k; ++i) {
                    if (algo.search(sources1, s1, (int)i)) {
                        FlipDistanceFPT algo2(s2, e2);
                        return algo2.search(sources2, s2, int(k - i));
                    }
                }
                return false;
            }
            g.flip(result);
        }
        std::vector<Edge> cur;
        std::unordered_multiset<Edge> forbid;
        std::function<bool(int)> generateNext = [&](int index) -> bool {
            if (index == sources.size()) {
                return search(cur, g, k);
            }
            if (generateNext(index + 1)) {
                return true;
            }
            for (const Edge &e: {sources[index].first, sources[index].second}) {
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
    
    static bool isIndependentSet(const std::vector<Edge> &sources, const TriangulatedGraph &g) {
        for (const Edge &e : sources) {
            for (const Edge &e2 : sources) {
                if (e != e2 && g.shareTriangle(e, e2)) {
                    return false;
                }
            }
        }
        return true;
    }

    bool search(const std::vector<Edge> &sources, TriangulatedGraph g,
                int k) { // keep as int; possible overflow for unsigned int
        branchCounter++;
        // sanity check
        for (const Edge &e: g.getEdges()) {
            Edge result = g.flip(e);
            g.flip(result);
            assert(!end.hasEdge(e));
            assert(!end.hasEdge(result));
        }
        assert(isIndependentSet(sources, g));
        
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
        std::vector<std::pair<Edge, Edge>> next;
        for (const Edge &e: sources) {
            assert(g.flippable(e));
            Edge result = g.flip(e);
            addNeighbors(next, g, result);
        }
        k -= (int) sources.size();
        return search(next, g, k);
    }

    bool flipDistanceDecision(unsigned int k, const std::vector<Edge> &source) {
        TriangulatedGraph g = start;
        return search(source, g, (int) k);
    }
    
public:

    bool flipDistanceDecision(unsigned int k) override {
        if (start == end) {
            return true;
        }
        TriangulatedGraph g = start;
        for (Edge e: g.getEdges()) {
            if (end.hasEdge(e)) {
                return splitAndSearch(g, e, (int) k, {});
            }
            Edge result = g.flip(e);
            if (end.hasEdge(result)) {
                bool ret = splitAndSearch(g, result, (int)k - 1, {});
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

#endif //FLIPDISTANCE_FLIP_DISTANCE_FPT_H
