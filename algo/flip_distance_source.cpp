//
// Created by Peter Li on 6/26/22.
//

#include "flip_distance_source.h"
#include "flip_distance_utils.h"
#include "statistics.h"
#include <algorithm>
#include <queue>
#include <unordered_set>

bool FlipDistanceSource::search(const std::vector<Edge> &sources, TriangulatedGraph g,
                                    int k) { // keep as int; possible overflow for unsigned int
    // sanity check
    assert(assertNonTrivial(g, end));
    assert(isIndependentSet(sources, g));

    if (g == end && k >= 0) {
        return true;
    }
    if ((int)g.getSize() - 3 > k - (int)sources.size()) {
        return false;
    }
    if (sources.empty()) {
        return false;
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

bool FlipDistanceSource::search(const std::vector<std::pair<Edge, Edge>> &sources, TriangulatedGraph g,
                                    int k) { // keep as int; possible overflow for unsigned int
    branchCounter++;
    // sanity check
    assert(assertNoCommonEdge(g, end));
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
            FlipDistanceSource algo(s1, e1);
            for (auto i = s1.getSize() - 3; i <= k; ++i) {
                if (algo.search(sources1, s1, (int)i)) {
                    FlipDistanceSource algo2(s2, e2);
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

bool FlipDistanceSource::splitAndSearch(const TriangulatedGraph &g,
                                        Edge &divider, int k) {
    if (k <= 0) {
        return g == end && k == 0;
    }
    int v1 = divider.first, v2 = divider.second;
    TriangulatedGraph s1 = g.subGraph(v1, v2), e1 = end.subGraph(v1, v2);
    TriangulatedGraph s2 = g.subGraph(v2, v1), e2 = end.subGraph(v2, v1);
    FlipDistanceSource algo(s1, e1);
    for (auto i = s1.getSize() - 3; i <= k; ++i) {
        if (algo.flipDistanceDecision(i)) {
            FlipDistanceSource algo2(s2, e2);
            return algo2.flipDistanceDecision(k - i);
        }
    }
    return false;
}

bool FlipDistanceSource::flipDistanceDecision(unsigned int k) {
    if (start == end) {
        return true;
    }
    TriangulatedGraph g = start;
    for (Edge e: g.getEdges()) {
        if (end.hasEdge(e)) {
            return splitAndSearch(g, e, (int) k);
        }
        Edge result = g.flip(e);
        if (end.hasEdge(result)) {
            bool ret = splitAndSearch(g, result, (int) k - 1);
            g.flip(result);
            return ret;
        }
        g.flip(result);
    }
    std::vector<std::vector<Edge>> sources = start.getSources();
    for (const auto &source: sources) {
        if (search(source, g, (int)k)) {
            if (DEBUG) {
                printf("Initial source count: %lu\n", source.size());
            }
            return true;
        }
    }
    return false;
}

std::vector<int64_t> FlipDistanceSource::getStatistics() {
    return {branchCounter, };
}
