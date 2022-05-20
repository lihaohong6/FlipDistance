//
// Created by Peter Li on 5/18/22.
//

#include "Helper.h"
#include <cassert>

int findMatchingFalse(const std::vector<bool> &bits, unsigned int index) {
    unsigned int counter = 1;
    for (; index < bits.size(); index++) {
        counter += bits[index] * 2 - 1;
        if (counter == 0) {
            return (int)index;
        }
    }
    return -1;
}

std::string binaryStringToTreeRep(const std::vector<bool> &bits, unsigned int indexStart, unsigned int indexEnd) {
    // empty string
    if (indexStart == indexEnd) {
        return "";
    }
    // single a
    if (indexStart == indexEnd - 2) {
        assert(bits[indexStart] == 1 && bits[indexStart + 1] == 0);
        return "a";
    }
    int matchingFalse = findMatchingFalse(bits, indexStart + 1);
    assert(matchingFalse != -1);
    // "a" followed by something
    if (matchingFalse == indexStart + 1) {
        return "a(" + binaryStringToTreeRep(bits, indexStart + 2, indexEnd) + ")";
    }
    // parentheses that cover the whole thing
    if (matchingFalse == indexEnd - 1) {
        return "(" + binaryStringToTreeRep(bits, indexStart + 1, indexEnd - 1) + ")a";
    }
    // parentheses half-way
    return "(" + binaryStringToTreeRep(bits, indexStart + 1, matchingFalse) + ")(" +
           binaryStringToTreeRep(bits, matchingFalse + 1, indexEnd) + ")";
}

void replaceAll(std::string &str, const std::string &from, const std::string &to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
}

std::string binaryStringToTreeRep(const std::vector<bool> &bits) {
    std::string s = binaryStringToTreeRep(bits, 0, bits.size());
    replaceAll(s, "(a)", "(aa)");
    return s;
}

std::string treeStringToParentheses(const std::string &s, size_t start, size_t end) {
    if (end == -1) {
        end = s.size();
    }
    if (start == s.size() || start >= end - 1) {
        return "";
    }
    if (s[start] == 'a') {
        return "()" + treeStringToParentheses(s, start + 1, end);
    } else {
        int counter = 1;
        size_t index = start + 1;
        while (counter > 0) {
            if (s[index] == ')') {
                counter--;
            } else if (s[index] == '(') {
                counter++;
            }
            index++;
        }
        index--;
        if (index == end - 1) {
            return treeStringToParentheses(s, start + 1, end - 1);
        }
        return "(" + treeStringToParentheses(s, start + 1, index) +
               ")" + treeStringToParentheses(s, index + 1, end);
    }
}

std::string treeStringToParentheses(const std::string &s) {
    return treeStringToParentheses(s, 0, s.size());
}

std::string triangulationGraphToTreeString(const TriangulatedGraph &g) {
    size_t size = g.getSize();
    std::string arr[size];
    for (const Node &v: g.vertices) {
        for (int neighbor: v.neighbors) {
            int diff = abs(v.id - neighbor);
            if (diff != 1 && diff != size - 1 && v.id < neighbor) {
                arr[v.id] += "(";
                arr[neighbor] = ")" + arr[neighbor];
            }
        }
    }
    std::string res;
    for (auto &s: arr) {
        res += s + 'a';
    }
    res.pop_back();
    return res;
}
