//
// Created by Peter Li on 4/12/22.
//

#ifndef FLIPDISTANCE_BINARYSTRING_H
#define FLIPDISTANCE_BINARYSTRING_H


#include <vector>
#include <bitset>
#include "TriangulatedGraph.h"

class TriangulatedGraph;

class BinaryString {
private:
    size_t size;
    std::vector<bool> bits;
public:
    explicit BinaryString(const std::string&);
    TriangulatedGraph toTriangulatedGraph() const;
    std::string toString();
    std::string toDyckPath(int digitWidth = 2) const;

    const std::vector<bool> &getBits() const;
};


#endif //FLIPDISTANCE_BINARYSTRING_H
