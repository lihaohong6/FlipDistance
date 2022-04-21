//
// Created by Peter Li on 4/17/22.
//

#include "gtest/gtest.h"
#include "../../triangulation/TriangulatedGraph.h"

TEST(TestTriangulationGraph, TestIsValid) {
    TriangulatedGraph g(5);
    EXPECT_FALSE(g.isValid());
    g.addEdge(0, 2);
    EXPECT_FALSE(g.isValid());
    g.addEdge(2, 4);
    EXPECT_TRUE(g.isValid());
    g.flip(0, 2);
    EXPECT_TRUE(g.isValid());
    g.flip(2, 4);
    EXPECT_TRUE(g.isValid());
}

TEST(TestTriangulationGraph, TestFlip) {
    TriangulatedGraph g(6);
    g.addEdge(0, 2);
    g.addEdge(0, 3);
    g.addEdge(0, 4);
    TriangulatedGraph g2(6);
    g2.addEdge(1, 4);
    g2.addEdge(1, 5);
    g2.addEdge(2, 4);
    g.flip(0, 3);
    g.flip(0, 2);
    ASSERT_FALSE(g == g2);
    g.flip(0, 4);
    EXPECT_TRUE(g == g2);
    g2.flip(2, 4);
    EXPECT_FALSE(g == g2);
    TriangulatedGraph g3(7);
    EXPECT_FALSE(g == g3);
}
