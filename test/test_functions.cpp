#include <gtest/gtest.h>
#include "../src/lib/Functions.hpp"

TEST(functions, FindCorrectDivisors){
    EXPECT_EQ(functions::find_opt_divisor(9), 3) << "Did not find correct divisor";
    EXPECT_EQ(functions::find_opt_divisor(40), 5) << "Did not find correct divisor";
    EXPECT_EQ(functions::find_opt_divisor(144), 12) << "Did not find correct divisors";
}
