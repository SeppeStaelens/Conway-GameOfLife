#include <gtest/gtest.h>
#include "../src/lib/Functions.hpp"

TEST(functions, FindCorrectDivisors){
    EXPECT_EQ(functions::find_largest_divisor(9, 5), 3) << "Did not find correct divisor";
}

TEST(functions, FindCorrectGridParameters){
    int d1, d2;
    std::tie(d1, d2) = functions::find_Cart_dim(42, 6);
    EXPECT_EQ(d1, 2) << "Did not find correct grid parameter";
    EXPECT_EQ(d2, 3) << "Did not find correct grid parameter";

    EXPECT_EXIT(functions::find_Cart_dim(42, 5), ::testing::ExitedWithCode(1), 
                "Error: Could not find suitable grid parameters.");
}
