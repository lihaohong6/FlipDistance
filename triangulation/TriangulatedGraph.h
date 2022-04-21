//
// Created by Peter Li on 4/12/22.
//

#ifndef FLIPDISTANCE_TRIANGULATEDGRAPH_H
#define FLIPDISTANCE_TRIANGULATEDGRAPH_H

#include <set>
#include "BinaryString.h"

class BinaryString;

struct Node {
    int id;
    std::set<int> neighbors;

    Node() : Node(0) {
    }

    explicit Node(int id) {
        this->id = id;
    }

    bool removeEdge(int a, int b);
};

typedef std::pair<int, int> Edge;

class TriangulatedGraph {
private:
    size_t size;
public:
    std::vector<Node> vertices;
    size_t getSize() const;
    
    explicit TriangulatedGraph(size_t size);

    void addEdge(int a, int b);
    
    bool hasEdge(int a, int b) const;
    
    std::vector<Edge> getNeighbors(Edge &e) const;
    
    bool flippable(const Edge &e) {
        int diff = abs(e.first - e.second);
        if (diff == 1 || diff == size - 1) {
            return false;
        }
        auto res = flip(e);
        if (res.first == -1) {
            return false;
        }
        flip(res);
        return true;
    }
    
    Edge flip(int a, int b);
    Edge flip(const Edge &e) {
        return flip(e.first, e.second);
    }

    BinaryString toBinaryString();
    
    bool isValid();

    bool operator==(const TriangulatedGraph &g) const;
};


#endif //FLIPDISTANCE_TRIANGULATEDGRAPH_H
