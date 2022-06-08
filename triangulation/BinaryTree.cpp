//
// Created by Peter Li on 6/8/22.
//

#include "BinaryTree.h"

void preOrderRecursive(Vertex* cur, std::vector<Vertex*> &accumulator) {
    if (cur == nullptr) {
        return;
    }
    cur->id = (int)accumulator.size();
    accumulator.push_back(cur);
    preOrderRecursive(cur->left, accumulator);
    preOrderRecursive(cur->right, accumulator);
}

std::vector<Vertex *> preOrder(Vertex *root) {
    std::vector<Vertex*> result;
    preOrderRecursive(root, result);
    return result;
}
