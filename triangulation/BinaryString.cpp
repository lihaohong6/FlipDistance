//
// Created by Peter Li on 4/12/22.
//

#include "BinaryString.h"
#include <algorithm>

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

TriangulatedGraph BinaryString::toTriangulatedGraph() const {
    TriangulatedGraph g(getBits());
    return g;
}

const std::vector<bool> &BinaryString::getBits() const {
    return bits;
}

std::string BinaryString::toDyckPath(int digitWidth) const {
    int height = 0, maxHeight = 0;
    for (bool bit : this->bits) {
        height += bit * 2 - 1;
        maxHeight = std::max(height, maxHeight);
    }
    auto maxWidth = this->size * digitWidth + 2;
    char result[maxHeight + 1][maxWidth];
    memset(result, ' ', sizeof(result));
    result[maxHeight][0] = '0';
    result[maxHeight][1] = '0';
    for (int bitIndex = 0; bitIndex < this->size; bitIndex++) {
        int bit = this->bits[bitIndex];
        height += bit * 2 - 1;
        int t = height;
        for (int i = 1; i <= digitWidth; ++i) {
            int row = maxHeight - height;
            int column = (bitIndex + 2) * 2 - i;
            char digit = (char)(t % 10 + 48);
            result[row][column] = digit;
            t /= 10;
        }
    }
    std::string ans;
    for (int i = 0; i <= maxHeight; ++i) {
        for (int j = 0; j < maxWidth; ++j) {
            ans += result[i][j];
        }
        ans += "\n";
    }
    return ans;
}


