//
// Created by Peter Li on 4/12/22.
//

#include "BinaryString.h"
#include "../utils/helper.h"

BinaryString::BinaryString(const std::string &s) {
    this->size = s.size();
    bits.resize(this->size);
    for (int i = 0; i < bits.size(); ++i) {
        bits[i] = s[i] == '(';
    }
}

std::string BinaryString::toString() {
    std::string res;
    for (bool b : this->bits) {
        res += b ? '(' : ')';
    }
    return res;
}

void addEdges(std::vector<Edge> list, std::vector<bool> bits, 
              size_t start, size_t end) {
    auto closing = findNext(bits, true, false, start + 1);
    // TODO: finish this
    return;
}

TriangulatedGraph BinaryString::toTriangulatedGraph() const {
    TriangulatedGraph g(getBits());
    return g;
}

const std::vector<bool> &BinaryString::getBits() const {
    return bits;
}


