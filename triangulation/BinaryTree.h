//
// Created by Peter Li on 6/7/22.
//

#ifndef FLIPDISTANCE_BINARYTREE_H
#define FLIPDISTANCE_BINARYTREE_H

#include "Edge.h"
#include <vector>

struct Vertex {
    int id = -1;
    Edge e;
    Vertex *top, *left, *right;
    
    explicit Vertex(const Edge &e) {
        this->e = e;
        top = left = right = nullptr;
    }

    Vertex() : Vertex(Edge(-1, -1)) {}
    
    ~Vertex() {
        delete left;
        delete right;
    }
};

std::vector<Vertex*> preOrder(Vertex* root);

#endif //FLIPDISTANCE_BINARYTREE_H
