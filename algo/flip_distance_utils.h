//
// Created by Peter Li on 6/28/22.
//

#ifndef FLIPDISTANCE_FLIP_DISTANCE_UTILS_H
#define FLIPDISTANCE_FLIP_DISTANCE_UTILS_H

#include "../triangulation/TriangulatedGraph.h"
#include <unordered_set>

static inline void addNeighbors(std::vector<std::pair<Edge, Edge>> &next,
                         const TriangulatedGraph &g, const Edge &e) {
    auto neighbors = g.getNeighbors(e);
    next.emplace_back(neighbors[0], neighbors[1]);
    next.emplace_back(neighbors[2], neighbors[3]);
}

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

std::vector<std::pair<Edge, Edge>>
static filterAndMapEdgePairs(const std::vector<std::pair<Edge, Edge>> &sources,
                      const std::function<bool(int)> &filter,
                      const std::function<int(int)> &mapper) {
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

static bool assertNoCommonEdge(const TriangulatedGraph &start, const TriangulatedGraph &end) {
    for (const Edge &e: start.getEdges()) {
        assert(!end.hasEdge(e));
    }
    return true;
}

static bool assertNoFreeEdge(const TriangulatedGraph &start, const TriangulatedGraph &end) {
    TriangulatedGraph g = start;
    for (const Edge &e: g.getEdges()) {
        Edge result = g.flip(e);
        g.flip(result);
        assert(!end.hasEdge(result));
    }
    return true;
}

static bool assertNonTrivial(const TriangulatedGraph &start, const TriangulatedGraph &end) {
    return assertNoCommonEdge(start, end) &&
           assertNoFreeEdge(start, end);
}

static bool isIndependentSet(const std::vector<Edge> &sources, const TriangulatedGraph &g) {
    for (const Edge &e: sources) {
        for (const Edge &e2: sources) {
            if (e != e2 && g.shareTriangle(e, e2)) {
                return false;
            }
        }
    }
    return true;
}

#endif //FLIPDISTANCE_FLIP_DISTANCE_UTILS_H
