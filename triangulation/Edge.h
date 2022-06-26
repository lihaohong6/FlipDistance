//
// Created by Peter Li on 6/7/22.
//

#ifndef FLIPDISTANCE_EDGE_H
#define FLIPDISTANCE_EDGE_H

#include <algorithm>

struct Edge {
    int first;
    int second;

    Edge() : Edge(-1, -1) {
    }

    Edge(int f, int s) {
        this->first = std::min(f, s);
        this->second = std::max(f, s);
    }

    int sum() const {
        return first + second;
    }

    friend bool operator==(const Edge &e1, const Edge &e2) {
        return e1.first == e2.first && e1.second == e2.second;
    }
    
    friend bool operator!=(const Edge &e1, const Edge &e2) {
        return !(e1 == e2);
    }

    friend bool operator<(const Edge &e1, const Edge &e2) {
        return e1.sum() - e2.sum() < 0;
    }
};

#endif //FLIPDISTANCE_EDGE_H
