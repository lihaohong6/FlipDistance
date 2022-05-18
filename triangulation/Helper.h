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

#endif //FLIPDISTANCE_HELPER_H
