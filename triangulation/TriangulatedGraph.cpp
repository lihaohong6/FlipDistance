//
// Created by Peter Li on 4/12/22.
//

#include "TriangulatedGraph.h"
#include "Helper.h"
#include <cassert>

TriangulatedGraph::TriangulatedGraph(size_t size) : size(size) {
    this->size = size;
    for (int i = 0; i < size; ++i) {
        vertices.emplace_back(i);
    }
    for (int i = 0; i < size; ++i) {
        addEdge(i, (i + 1) % size);
    }
}

TriangulatedGraph::TriangulatedGraph(const std::vector<bool> &bits)
        : TriangulatedGraph(bits.size() / 2 + 2) {
    std::string s = binaryStringToTreeRep(bits);
    std::vector<int> stack;
    int currVertex = 0;
    // char prev = ' ';
    for (char c: s) {
        if (c == 'a') {
            currVertex += 1;
            // currVertex += c != prev;
        } else if (c == '(') {
            stack.push_back(currVertex);
        } else if (c == ')') {
            int v = stack[stack.size() - 1];
            stack.pop_back();
            addEdge(v, currVertex);
        } else {
            assert(false);
        }
        // prev = c;
    }
}

void TriangulatedGraph::addEdge(int a, int b) {
    vertices[a].neighbors.insert(b);
    vertices[b].neighbors.insert(a);
}

void getSharedNeighbors(const TriangulatedGraph *g, const Node *v1, const Node *v2, std::vector<int> &result) {
    std::vector<int> neighbor1(g->getSize()), neighbor2(g->getSize());
    for (int n: v1->neighbors) {
        neighbor1[n] = 1;
    }
    for (int n: v2->neighbors) {
        neighbor2[n] = 1;
    }
    for (int i = 0; i < g->getSize(); i++) {
        if (neighbor1[i] == neighbor2[i] && neighbor1[i] == 1) {
            result.push_back(i);
        }
    }
}

Edge TriangulatedGraph::flip(const int a, const int b) {
    Node *v1 = &vertices[a], *v2 = &vertices[b];
    std::vector<int> sharedNeighbors;
    getSharedNeighbors(this, v1, v2, sharedNeighbors);
    if (sharedNeighbors.size() != 2) {
        return {-1, -1};
    }
    v1->removeEdge(a, b);
    v2->removeEdge(a, b);
    int n1 = sharedNeighbors[0], n2 = sharedNeighbors[1];
    addEdge(n1, n2);
    return {n1, n2};
}

//std::string toStringRecursive(TriangulatedGraph &g, int start, int end) {
//    
//}

BinaryString TriangulatedGraph::toBinaryString() const {
    // TODO: use more intuitive splitting approach
    return BinaryString(treeStringToParentheses(triangulationGraphToTreeString(*this)));
}

size_t TriangulatedGraph::getSize() const {
    return size;
}

bool TriangulatedGraph::isValid() {
    size_t total = 0;
    for (const Node &v: vertices) {
        total += v.neighbors.size();
    }
    return total / 2 == size * 2 - 3;
}

bool TriangulatedGraph::operator==(const TriangulatedGraph &g) const {
    if (size != g.getSize()) {
        return false;
    }
    for (const Node &v1: vertices) {
        const Node *v2 = &g.vertices[v1.id];
        for (int neighbor: v1.neighbors) {
            if (!v2->neighbors.count(neighbor)) {
                return false;
            }
        }
    }
    return true;
}

bool TriangulatedGraph::hasEdge(int a, int b) const {
    assert(a >= 0 && a < size && b >= 0 && b < size);
    return vertices[a].neighbors.count(b);
}

std::vector<Edge> TriangulatedGraph::getNeighbors(const Edge &e) const {
    std::vector<Edge> edges;
    std::vector<int> neighbors;
    getSharedNeighbors(this, &vertices[e.first], &vertices[e.second], neighbors);
    edges.emplace_back(e.first, neighbors[0]);
    edges.emplace_back(e.second, neighbors[0]);
    edges.emplace_back(e.first, neighbors[1]);
    edges.emplace_back(e.second, neighbors[1]);
    return edges;
}

bool TriangulatedGraph::flippable(const Edge &e) {
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

std::vector<bool> TriangulatedGraph::toVector() const {
    return toBinaryString().getBits();
}

std::vector<Edge> TriangulatedGraph::getEdges() const {
    std::vector<Edge> result;
    for (const Node &v: vertices) {
        for (int e: v.neighbors) {
            if (v.id < e && !isSimpleEdge(e, v.id)) {
                result.emplace_back(v.id, e);
            }
        }
    }
    return result;
}

bool TriangulatedGraph::shareTriangle(const Edge &e1, const Edge &e2) const {
    if (e1.first == e2.first) {
        return hasEdge(e1.second, e2.second);
    }
    if (e1.first == e2.second) {
        return hasEdge(e1.second, e2.first);
    }
    if (e1.second == e2.first) {
        return hasEdge(e1.first, e2.second);
    }
    if (e1.second == e2.second) {
        return hasEdge(e1.first, e2.first);
    }
    return false;
}

bool TriangulatedGraph::isSimpleEdge(int a, int b) const {
    return abs(a - b) == 1 || abs(a - b) == size - 1;
}

bool Node::removeEdge(const int a, const int b) {
    int other = a == this->id ? b : (b == this->id ? a : -1);
    if (other == -1) {
        return false;
    }
    neighbors.erase(other);
    return true;
}
