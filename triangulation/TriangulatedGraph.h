//
// Created by Peter Li on 4/12/22.
//

#ifndef FLIPDISTANCE_TRIANGULATEDGRAPH_H
#define FLIPDISTANCE_TRIANGULATEDGRAPH_H

#include <set>
#include "BinaryString.h"
#include <vector>
#include "BinaryTree.h"
#include "Edge.h"

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

// typedef std::pair<int, int> Edge;

class TriangulatedGraph {
private:
    size_t size;
public:
    std::vector<Node> vertices;

    size_t getSize() const;

    explicit TriangulatedGraph(size_t size);

    explicit TriangulatedGraph(const std::vector<bool> &bits);

    void addEdge(int a, int b);
    void addEdge(Edge e) {
        addEdge(e.first, e.second);
    }

    bool hasEdge(int a, int b) const;

    bool hasEdge(Edge e) const {
        return hasEdge(e.first, e.second);
    }

    std::vector<Edge> getNeighbors(const Edge &e, bool includeBoundary = false) const;

    bool flippable(const Edge &e);

    Edge flip(int a, int b);

    Edge flip(const Edge &e) {
        return flip(e.first, e.second);
    }

    bool shareTriangle(const Edge &e1, const Edge &e2) const;

    BinaryString toBinaryString() const;

    std::vector<bool> toVector() const;
    
    Vertex* toBinaryTree() const;

    bool isValid();

    bool isSimpleEdge(int a, int b) const;
    bool isSimpleEdge(Edge e) const {
        return isSimpleEdge(e.first, e.second);
    }

    std::vector<Edge> getEdges() const;

    bool operator==(const TriangulatedGraph &g) const;

    std::vector<std::vector<Edge>> getSources() const;

    std::vector<Edge> filterAndMapEdges(int start, int end, const std::vector<Edge> &edges) const;
    
    TriangulatedGraph subGraph(int start, int end) const;
};

#endif //FLIPDISTANCE_TRIANGULATEDGRAPH_H
