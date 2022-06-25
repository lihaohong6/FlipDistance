//
// Created by Peter Li on 4/12/22.
//

#include "TriangulatedGraph.h"
#include "Helper.h"
#include "BinaryTree.h"
#include "../config.h"
#include <cassert>
#include <unordered_set>

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
    assert(0 <= a && a < size);
    assert(0 <= b && b < size);
    vertices[a].neighbors.insert(b);
    vertices[b].neighbors.insert(a);
}

std::vector<int> getSharedNeighbors(const TriangulatedGraph *g, const Node *v1, const Node *v2) {
    std::vector<int> result;
    std::vector<bool> neighbor1(g->getSize()), neighbor2(g->getSize());
    for (int n: v1->neighbors) {
        neighbor1[n] = true;
    }
    for (int n: v2->neighbors) {
        neighbor2[n] = true;
    }
    for (int i = 0; i < g->getSize(); i++) {
        if (neighbor1[i] && neighbor2[i]) {
            result.push_back(i);
        }
    }
    return result;
}

Edge TriangulatedGraph::flip(const int a, const int b) {
    Node *v1 = &vertices[a], *v2 = &vertices[b];
    std::vector<int> sharedNeighbors = getSharedNeighbors(this, v1, v2);
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

std::vector<Edge> TriangulatedGraph::getNeighbors(const Edge &e, bool includeBoundary) const {
    std::vector<Edge> edges;
    std::vector<int> neighbors = getSharedNeighbors(this, &vertices[e.first], &vertices[e.second]);
    edges.emplace_back(e.first, neighbors[0]);
    edges.emplace_back(e.second, neighbors[0]);
    if (neighbors.size() > 1) {
        edges.emplace_back(e.first, neighbors[1]);
        edges.emplace_back(e.second, neighbors[1]);
    }
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

size_t findMatchingParenthesis(const std::string &s, int index) {
    return findNext(s.begin() + index, s.end(), '(', ')', index);
}

std::string substr(const std::string &s, size_t start, size_t end = -1) {
    if (end == -1) {
        end = s.length();
    }
    return s.substr(start, end - start);
}

std::string preprocess(const std::string &s) {
    if (s[0] == 'a') {
        return s;
    }
    auto match = findMatchingParenthesis(s, 1);
    if (match == s.length() - 1) {
        return substr(s, 1, s.length() - 1);
    }
    return s;
}

Vertex *buildVertex(const std::string &s) {
    if (s == "a") {
        return nullptr;
    }
    auto *v = new Vertex();
    std::string left, right;
    if (s[0] != 'a') {
        auto split = findMatchingParenthesis(s, 1);
        left = substr(s, 1, split);
        right = preprocess(substr(s, split + 1));
    } else {
        if (s.length() > 1 && s[1] == 'a') {
            left = right = "a";
        } else {
            left = "a";
            right = preprocess(substr(s, 1));
        }
    }
    v->left = buildVertex(left);
    v->right = buildVertex(right);
    return v;
}

void sortEdge(Edge &e1, Edge &e2) {
    if (e2 < e1) {
        std::swap(e1, e2);
    }
}

void assignEdge(const TriangulatedGraph &g, Vertex *v, const Edge &e, Vertex *prev) {
    if ((v == nullptr) ^ g.isSimpleEdge(e)) {
        assert(false);
    }
    if ((v == nullptr) && g.isSimpleEdge(e)) {
        return;
    }
    v->top = prev;
    v->e = e;
    auto neighbors = g.getNeighbors(e);
    int index = 0;
    for (auto neighbor: neighbors) {
        if (neighbor == prev->e) {
            Edge e1, e2;
            if (index < 2) {
                e1 = neighbors[2];
                e2 = neighbors[3];
            } else {
                e1 = neighbors[0];
                e2 = neighbors[1];
            }
            sortEdge(e1, e2);
            assignEdge(g, v->left, e1, v);
            assignEdge(g, v->right, e2, v);
            return;
        }
        index++;
    }
    assert(false);
}

bool canSelect(Vertex *v, std::vector<bool> &selections) {
    if (v->top == nullptr) {
        return false;
    }
    if (selections[v->top->id]) {
        return false;
    }
    if (v->top->left != nullptr && selections[v->top->left->id]) {
        return false;
    }
    return true;
}

void independentSet(std::vector<std::vector<Edge>> &accumulator,
                    std::vector<Vertex *> &vertices, std::vector<bool> &selections, int index) {
    if (index == vertices.size()) {
        std::vector<Edge> result;
        for (int i = 0; i < selections.size(); ++i) {
            if (selections[i]) {
                result.push_back(vertices[i]->e);
            }
        }
        accumulator.push_back(result);
        return;
    }
    Vertex *v = vertices[index];
    if (canSelect(v, selections)) {
        selections[index] = true;
        independentSet(accumulator, vertices, selections, index + 1);
        selections[index] = false;
    }
    independentSet(accumulator, vertices, selections, index + 1);
}

std::vector<std::vector<Edge>> TriangulatedGraph::getSources() const {
    using namespace std;
    Vertex *root = toBinaryTree();
    Edge start(0, size - 1);
    root->e = start;
    auto neighbors = getNeighbors(start);
    assert(neighbors.size() == 2);
    Edge left = neighbors[0], right = neighbors[1];
    sortEdge(left, right);
    assignEdge(*this, root->left, left, root);
    assignEdge(*this, root->right, right, root);
    vector<vector<Edge>> accumulator;
    int totalNodes = this->size - 2;
    std::vector<Vertex *> list = preOrder(root);
    std::vector<bool> selection(totalNodes);
    independentSet(accumulator, list, selection, 1);
    return accumulator;
}

Vertex *TriangulatedGraph::toBinaryTree() const {
    return buildVertex(triangulationGraphToTreeString(*this));
}

TriangulatedGraph TriangulatedGraph::subGraph(int start, int end) const {
    int newSize = start <= end ? end - start + 1 : (int) size - (start - end + 1) + 2;
    TriangulatedGraph result(newSize);
    std::vector<Edge> edges = filterAndMapEdges(start, end, getEdges());
    for (Edge e: edges) {
        result.addEdge(e);
    }
    return result;
}

std::vector<Edge> TriangulatedGraph::filterAndMapEdges(int start, int end, const std::vector<Edge> &edges) const {
    std::vector<Edge> result;
    auto predicate = [&](int e) -> bool {
        if (start <= end) {
            return start <= e && e <= end;
        }
        return start <= e || e <= end;
    };
    auto mapper = [&](int v) -> int {
        if (start <= end) {
            return v - start;
        }
        return v >= start ? v - start : v + (int) size - start;
    };
    for (Edge e: edges) {
        if (!isSimpleEdge(e) && predicate(e.first) && predicate(e.second)) {
            result.emplace_back(mapper(e.first), mapper(e.second));
        }
    }
    return result;
}

bool Node::removeEdge(const int a, const int b) {
    int other = a == this->id ? b : (b == this->id ? a : -1);
    if (other == -1) {
        return false;
    }
    neighbors.erase(other);
    return true;
}
