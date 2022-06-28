//
// Created by Peter Li on 6/28/22.
//

#ifndef FLIPDISTANCE_FLIP_DISTANCE_SOURCE_FAST_H
#define FLIPDISTANCE_FLIP_DISTANCE_SOURCE_FAST_H

#include <vector>
#include "flip_distance_source.h"

class FlipDistanceSourceFast : public FlipDistanceSource {

public:

    FlipDistanceSourceFast(TriangulatedGraph start, TriangulatedGraph end)
            : FlipDistanceSource(std::move(start), std::move(end)) {}
            
    bool search(const std::vector<std::pair<Edge, Edge>> &sources, TriangulatedGraph g,
                int k) override;

    bool search(const std::vector<Edge> &sources, TriangulatedGraph g,
                int k) override;
};

#endif //FLIPDISTANCE_FLIP_DISTANCE_SOURCE_FAST_H
