//
// Created by Peter Li on 3/21/23.
//

#include "gtest/gtest.h"
#include "../../triangulation/BinaryString.h"

TEST(TestBinaryString, TestToDyckPath) {
    BinaryString s("(())()");
    auto expected = std::string() + 
            "    02        \n" +
            "  01  01  01  \n" +
            "00      00  00\n";
    EXPECT_EQ(expected, s.toDyckPath());
}
