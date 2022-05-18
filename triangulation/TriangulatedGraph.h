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
    explicit TriangulatedGraph(const std::vector<bool> &bits);

    void addEdge(int a, int b);
    
    bool hasEdge(int a, int b) const;
    bool hasEdge(Edge e) const {
        return hasEdge(e.first, e.second);
    }
    
    std::vector<Edge> getNeighbors(Edge &e) const;
    
    bool flippable(const Edge &e);
    
    Edge flip(int a, int b);
    Edge flip(const Edge &e) {
        return flip(e.first, e.second);
    }

    BinaryString toBinaryString() const;
    std::vector<bool> toVector();
    
    bool isValid();
    
    std::vector<Edge> getEdges();

    bool operator==(const TriangulatedGraph &g) const;
};


#endif //FLIPDISTANCE_TRIANGULATEDGRAPH_H
