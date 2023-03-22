//
// Created by Peter Li on 4/20/22.
//

#include "gtest/gtest.h"
#include "../../algo/flip_distance_bfs.h"
#include "../../algo/flip_distance_source.h"
#include "../../triangulation/Helper.h"
#include "../../utils/rand.h"

void assertFd(TriangulatedGraph &g1, TriangulatedGraph &g2, int distance, int max) {
    FlipDistanceSource fd(g1, g2);
    for (int i = 1; i < distance; i++) {
        ASSERT_FALSE(fd.flipDistanceDecision(i));
    }
    for (int i = distance; i <= max; i++) {
        ASSERT_TRUE(fd.flipDistanceDecision(i));
    }
    ASSERT_EQ(distance, fd.flipDistance());
}

TEST(TestFlipDistance, TestFlipDistance_withSquare) {
    TriangulatedGraph g(4);
    g.addEdge(0, 2);
    TriangulatedGraph g2(4);
    g2.addEdge(1, 3);
    assertFd(g, g2, 1, 3);
}

TEST(TestFlipDistance, TestFlipDistance_withMultipleFlips) {
    TriangulatedGraph g(6);
    g.addEdge(0, 2);
    g.addEdge(0, 3);
    g.addEdge(0, 4);
    TriangulatedGraph g2(6);
    g2.addEdge(1, 3);
    g2.addEdge(1, 5);
    g2.addEdge(3, 5);
    assertFd(g, g2, 3, 5);
    g2.flip(1,3);
    assertFd(g, g2, 3, 5);
}

TEST(TestFlipDistance, TestFlipDistance_withMultiplePaths) {
    TriangulatedGraph g(8);
    g.addEdge(0, 5);
    g.addEdge(0, 3);
    g.addEdge(0, 4);
    g.addEdge(1, 3);
    g.addEdge(5, 7);
    TriangulatedGraph g2(8);
    g2.addEdge(0, 5);
    g2.addEdge(0, 6);
    g2.addEdge(1, 5);
    g2.addEdge(2, 4);
    g2.addEdge(2, 5);
    auto sub1 = g.subGraph(0, 5), sub2 = g2.subGraph(0, 5);
    assertFd(sub1, sub2, 4, 5);
    assertFd(g, g2, 5, 6);
    g.flip(0, 3);
    assertFd(g, g2, 4, 6);
}

TEST(TestFlipDistance, TestFlipDistance_with8gon) {
    TriangulatedGraph g(8);
    g.addEdge(0, 2);
    g.addEdge(0, 3);
    g.addEdge(3, 5);
    g.addEdge(3, 7);
    g.addEdge(5, 7);
    TriangulatedGraph g2(8);
    g2.addEdge(1, 6);
    g2.addEdge(1, 7);
    g2.addEdge(2, 4);
    g2.addEdge(2, 6);
    g2.addEdge(4, 6);
    assertFd(g, g2, 7, 10);
}

TEST(TestFlipDistance, TestFlipDistance_with10gon) {
    TriangulatedGraph g(10);
    g.addEdge(0, 3);
    g.addEdge(0, 4);
    g.addEdge(1, 3);
    g.addEdge(4, 9);
    g.addEdge(5, 7);
    g.addEdge(5, 9);
    g.addEdge(9, 7);
    TriangulatedGraph g2(10);
    g2.addEdge(1, 6);
    g2.addEdge(1, 8);
    g2.addEdge(1, 9);
    g2.addEdge(2, 4);
    g2.addEdge(2, 5);
    g2.addEdge(2, 6);
    g2.addEdge(6, 8);
    assertFd(g, g2, 10, 11);
}

void testFdStr(const std::string& s1, const std::string& s2, int expected) {
    TriangulatedGraph 
        g1(BinaryString(treeStringToParentheses(s1)).getBits()),
        g2(BinaryString(treeStringToParentheses(s2)).getBits());
    FlipDistanceBfs fd(g1, g2);
    ASSERT_EQ(fd.flipDistance(), expected);
}

TEST(TestFlipDistance, TestFlipDistance_with14gon) {
    testFdStr("(((a((a((aa)a))a))a)(a(a(aa))))(aa)", "(a(((a((a(a(((aa)a)a)))a))a)(aa)))a", 15);
}

TEST(TestFlipDistance, TestFlipDistance_withRandom12gon) {
    int repetitions = 5;
    while(repetitions--) {
        auto pair = randomTriangulation(12, true);
        FlipDistanceSource fd(pair.first, pair.second);
        FlipDistanceBfs fd2(pair.first, pair.second);
        ASSERT_EQ(fd.flipDistance(), fd2.flipDistance());
    }
}
