#include <gtest/gtest.h>
#include <variables.h>

TEST(Value, works)
{
  EXPECT_NEAR(std::log2(value(1542, false)), std::log2(2.740e47), 0.001);
  EXPECT_NEAR(std::log2(value(1528, true)), std::log2(1.028e47), 0.001);
}
