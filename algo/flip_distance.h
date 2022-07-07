//
// Created by Peter Li on 4/23/22.
//

#ifndef FLIPDISTANCE_FLIP_DISTANCE_H
#define FLIPDISTANCE_FLIP_DISTANCE_H

#include "../triangulation/TriangulatedGraph.h"
#include <cassert>

class FlipDistance {
protected:
    const TriangulatedGraph start;
    const TriangulatedGraph end;
public:
    FlipDistance(TriangulatedGraph start, TriangulatedGraph end) : start(std::move(start)), end(std::move(end)) {}
    
    bool isTrivial() {
        // TODO: finish this
        return start == end;
    }
    
    virtual bool flipDistanceDecision(unsigned int k) {
        return false;
    };
    
    unsigned int flipDistance(unsigned int min, unsigned int max) {
        for (auto i = min; i <= max; ++i) {
            if (flipDistanceDecision(i)) {
                return i;
            }
        }
        assert(false);
    }

    virtual unsigned int flipDistance() {
        return flipDistance(0, start.getSize() * 2 - 6);
    }
    
    virtual std::vector<int64_t> getStatistics() {
        return {};
    }
};

#endif //FLIPDISTANCE_FLIP_DISTANCE_H
