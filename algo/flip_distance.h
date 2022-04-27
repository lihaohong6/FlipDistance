//
// Created by Peter Li on 4/23/22.
//

#ifndef FLIPDISTANCE_FLIP_DISTANCE_H
#define FLIPDISTANCE_FLIP_DISTANCE_H

struct Action {
    const int type;
    const Edge edge;

    Action(int type, Edge edgeStart) : type(type), edge(std::move(edgeStart)) {}
};

class FlipDistance {
protected:
    const TriangulatedGraph start;
    const TriangulatedGraph end;
public:
    FlipDistance(TriangulatedGraph start, TriangulatedGraph end) : start(std::move(start)), end(std::move(end)) {}
    
    virtual bool flipDistanceDecision(unsigned int k) {
        return false;
    };

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
