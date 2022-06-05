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
public:
    FlipDistanceSource(TriangulatedGraph start, TriangulatedGraph end)
            : FlipDistance(std::move(start), std::move(end)) {}

    void addNeighborsToForbid(const Edge &e, const TriangulatedGraph &g,
                              std::unordered_multiset<Edge, HashEdge> &forbid) {
        for (const Edge &neighbor: g.getNeighbors(e)) {
            forbid.insert(neighbor);
        }
    }

    void removeNeighborsFromForbid(const Edge &e, const TriangulatedGraph &g,
                                   std::unordered_multiset<Edge, HashEdge> &forbid) {
        for (const Edge &neighbor: g.getNeighbors(e)) {
            forbid.erase(neighbor);
        }
    }

    void generateSources(const std::vector<Edge> &edges, int index, const TriangulatedGraph &g,
                         std::vector<Edge> &cur, std::unordered_multiset<Edge, HashEdge> &forbid,
                         std::vector<std::vector<Edge>> &existing) {
        if (index == edges.size()) {
            existing.push_back(cur);
            return;
        }
        bool canAdd = !forbid.count(edges[index]);
        generateSources(edges, index + 1, g, cur, forbid, existing);
        if (canAdd) {
            auto q2 = cur;
            q2.push_back(edges[index]);
            addNeighborsToForbid(edges[index], g, forbid);
            generateSources(edges, index + 1, g, q2, forbid, existing);
            removeNeighborsFromForbid(edges[index], g, forbid);
        }
    }

    void generateSources(const std::vector<Edge> &edges, const TriangulatedGraph &g,
                         std::vector<std::vector<Edge>> &existing) {
        std::vector<Edge> empty;
        std::unordered_multiset<Edge, HashEdge> forbid;
        generateSources(edges, 0, g, empty, forbid, existing);
    }

    template<class InputIterator>
    void generateSources(InputIterator b, InputIterator e, const TriangulatedGraph &g,
                         std::vector<std::vector<Edge>> &existing) {
        std::vector<Edge> edges;
        for (; b != e; ++b) {
            edges.push_back(*b);
        }
        generateSources(edges, g, existing);
    }

    void generateNext(const std::vector<std::pair<Edge, Edge>> &edges, int index,
                      TriangulatedGraph &g, std::vector<Edge> &cur,
                      std::unordered_multiset<Edge, HashEdge> &forbid,
                      std::vector<std::vector<Edge>> &accumulator) {
        if (index == edges.size()) {
            accumulator.push_back(cur);
            return;
        }
        generateNext(edges, index + 1, g, cur, forbid, accumulator);
        for (const Edge &e: {edges[index].first, edges[index].second}) {
            if (!g.flippable(e)) {
                continue;
            }
            Edge res = g.flip(e);
            addNeighborsToForbid(res, g, forbid);
            auto next = cur;
            next.push_back(e);
            generateNext(edges, index + 1, g, next, forbid, accumulator);
            removeNeighborsFromForbid(res, g, forbid);
            g.flip(res);
        }
    }

    bool search(const std::vector<Edge> &sources, TriangulatedGraph g, int k) {
        if (g == end && k >= 0) {
            return true;
        }
        if (k <= 0) {
            return false;
        }
        if (sources.empty()) {
            return false;
        }
        std::vector<Edge> freeEdges;
        std::unordered_multiset<Edge, HashEdge> forbid;
        TriangulatedGraph g2 = g;
        auto flipped = 0;
//        for (const Edge &e : g2.getEdges()) {
//            Edge res = g.flip(e);
//            if (!end.hasEdge(res)) {
//                g.flip(res);
//                continue;
//            }
//            flipped++;
//            addNeighborsToForbid(res, g, forbid);
//        }
        std::vector<std::pair<Edge, Edge>> next;
        std::vector<std::vector<Edge>> accumulator;
        for (const Edge &e: sources) {
            if (g.flippable(e)) {
                Edge result = g.flip(e);
                flipped++;
                auto neighbors = g.getNeighbors(result);
                for (const Edge &neighbor: neighbors) {
                    if (!g.flippable(neighbor)) {
                        continue;
                    }
                    Edge res = g.flip(neighbor);
                    if (end.hasEdge(res)) {
                        freeEdges.push_back(neighbor);
                        addNeighborsToForbid(neighbor, g, forbid);
                    }
                    g.flip(res);
                }
                next.emplace_back(neighbors[0], neighbors[1]);
                next.emplace_back(neighbors[2], neighbors[3]);
            }
        }
        generateNext(next, 0, g, freeEdges, forbid, accumulator);
        for (const auto &edges: accumulator) {
            if (search(edges, g, k - flipped)) {
                return true;
            }
        }
        return false;
    }

    bool flipDistanceDecision(unsigned int k) override {
        std::vector<std::vector<Edge>> sources;
        generateSources(start.getEdges(), start, sources);
        for (auto &source: sources) {
            if (search(source, start, (int)k)) {
                return true;
            }
        }
        return false;
    }
};

#endif //FLIPDISTANCE_FLIP_DISTANCE_SOURCE_H
