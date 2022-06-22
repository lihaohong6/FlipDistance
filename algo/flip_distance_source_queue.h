//
// Created by Peter Li on 6/4/22.
//

#ifndef FLIPDISTANCE_FLIP_DISTANCE_SOURCE2_H
#define FLIPDISTANCE_FLIP_DISTANCE_SOURCE2_H

#include "flip_distance.h"
#include <algorithm>
#include <queue>

class FlipDistanceSource2 : public FlipDistance {
public:
    FlipDistanceSource2(TriangulatedGraph start, TriangulatedGraph end)
            : FlipDistance(std::move(start), std::move(end)) {}

    bool search(std::queue<Edge> &queue, TriangulatedGraph &t, unsigned int remainingFlips) {
        if (t == end) {
            return true;
        }
        if (remainingFlips == 0) {
            return false;
        }
        if (queue.empty()) {
            return false;
        }
        if (!t.flippable(queue.front())) {
            return false;
        }
        Edge front = queue.front();
        Edge result = t.flip(front);
        queue.pop();
        std::vector<Edge> neighbors = t.getNeighbors(result);
//        for (Edge e : neighbors) {
//            Edge r = t.flip(e);
//            if (r.first != -1) {
//                if (end.hasEdge(r)) {
//                    bool ans = search(queue, t, remainingFlips - 2);
//                    t.flip(r);
//                    t.flip(front);
//                    return ans;
//                }
//            }
//        }
        std::vector<std::vector<Edge>> addToQueue;
        // case 1: this is a sink
        addToQueue.emplace_back();
        // case 2: add one edge
        for (Edge e: neighbors) {
            std::vector<Edge> v;
            v.push_back(e);
            addToQueue.push_back(v);
        }
        // case 3: add two edges
        std::vector<Edge> v;
        v.push_back(neighbors[0]);
        v.push_back(neighbors[1]);
        addToQueue.push_back(v);
        v.clear();
        v.push_back(neighbors[2]);
        v.push_back(neighbors[3]);
        addToQueue.push_back(v);
        bool ans = false;
        for (const auto &add: addToQueue) {
            std::queue<Edge> next = queue;
            for (Edge e: add) {
                next.push(e);
            }
            if (search(next, t, remainingFlips - 1)) {
                ans = true;
                break;
            }
        }
        t.flip(result);
        return ans;
    }

    void generateQueues(const std::vector<Edge> &edges, int index,
                        std::queue<Edge> &cur,
                        std::vector<std::queue<Edge>> &existing) {
        if (index == edges.size()) {
            existing.push_back(cur);
            return;
        }
        bool canAdd = true;
        for (auto q2 = cur; !q2.empty(); q2.pop()) {
            if (start.shareTriangle(edges[index], q2.front())) {
                canAdd = false;
                break;
            }
        }
        generateQueues(edges, index + 1, cur, existing);
        if (canAdd) {
            auto q2 = cur;
            q2.push(edges[index]);
            generateQueues(edges, index + 1, q2, existing);
        }
    }

    bool flipDistanceDecision(unsigned int k) override {
        std::vector<std::queue<Edge>> queues;
        std::queue<Edge> q;
        generateQueues(start.getEdges(), 0, q, queues);
        for (std::queue<Edge> &queue: queues) {
            auto g = start;
            if (search(queue, g, k)) {
                return true;
            }
        }
        return false;
    }
};

#endif //FLIPDISTANCE_FLIP_DISTANCE_SOURCE2_H
