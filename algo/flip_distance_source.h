//
// Created by Peter Li on 6/4/22.
//

#ifndef FLIPDISTANCE_FLIP_DISTANCE_SOURCE_H
#define FLIPDISTANCE_FLIP_DISTANCE_SOURCE_H

#include "flip_distance.h"
#include <vector>

namespace std {
    template<>
    struct [[maybe_unused]] hash<Edge> {
        size_t operator()(const Edge &e) const {
            return std::hash<int>()(e.first) * 31 + std::hash<int>()(e.second);
        }
    };

}

class FlipDistanceSource : public FlipDistance {

public:

    FlipDistanceSource(TriangulatedGraph start, TriangulatedGraph end)
            : FlipDistance(std::move(start), std::move(end)) {}

    bool splitAndSearch(const TriangulatedGraph &g, Edge &divider,
                        int k, // keep as int; possible overflow for unsigned int
                        const std::vector<Edge> &sources);

    bool search(const std::vector<std::pair<Edge, Edge>> &sources, TriangulatedGraph g,
                int k);
    
    bool search(const std::vector<Edge> &sources, TriangulatedGraph g,
                int k);

    bool flipDistanceDecision(unsigned int k, const std::vector<Edge> &source) {
        TriangulatedGraph g = start;
        return search(source, g, (int) k);
    }

    bool flipDistanceDecision(unsigned int k) override;

    std::vector<int> getStatistics() override;
};

void resetBranchCounter();

#endif //FLIPDISTANCE_FLIP_DISTANCE_SOURCE_H
