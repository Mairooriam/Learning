
#include <gtest/gtest.h>
#include "example.hpp"

// Test case for add function
TEST(ExampleTest, Add) {
    EXPECT_EQ(add(1, 1), 2);
    EXPECT_EQ(add(-1, 1), 0);
    EXPECT_EQ(add(-1, -1), -2);
}

// Test case for isEven function
TEST(ExampleTest, IsEven) {
    EXPECT_TRUE(isEven(2));
    EXPECT_FALSE(isEven(3));
    EXPECT_TRUE(isEven(0));
    EXPECT_FALSE(isEven(-1));
    EXPECT_TRUE(isEven(-2));
}

// Main function for Google Test
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}