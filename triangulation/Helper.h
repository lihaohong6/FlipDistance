//
// Created by Peter Li on 5/18/22.
//

#ifndef FLIPDISTANCE_HELPER_H
#define FLIPDISTANCE_HELPER_H

#include <string>
#include <vector>
#include "TriangulatedGraph.h"

std::string binaryStringToTreeRep(const std::vector<bool> &bits);

std::string triangulationGraphToTreeString(const TriangulatedGraph &g);

std::string treeStringToParentheses(const std::string &s);

template<typename T, class InputIterator>
size_t findNext(InputIterator s, InputIterator e, const T a, const T b, size_t start) {
    for (int counter = 1; s != e; ++s, ++start) {
        if (*s == a) {
            counter++;
        } else if (*s == b) {
            counter--;
        }
        if (counter == 0) {
            return start;
        }
    }
    return -1;
}

#endif //FLIPDISTANCE_HELPER_H
