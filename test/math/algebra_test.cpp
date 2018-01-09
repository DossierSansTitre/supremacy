#include <gtest/gtest.h>
#include <math/algebra.h>

TEST(MathMinTest, EqualValues)
{
    ASSERT_EQ(min(0, 0), 0);
    ASSERT_EQ(min(-25, -25), -25);
    ASSERT_EQ(min(34, 34), 34);
}

TEST(MathMinTest, DifferentValues)
{
    ASSERT_EQ(min(1, 2), 1);
    ASSERT_EQ(min(1, 10), 1);
    ASSERT_EQ(min(429, 0), 0);
    ASSERT_EQ(min(2, -3), -3);
}

TEST(MathMaxTest, EqualValues)
{
    ASSERT_EQ(max(0, 0), 0);
    ASSERT_EQ(max(-25, -25), -25);
    ASSERT_EQ(max(34, 34), 34);
}

TEST(MathMaxTest, DifferentValues)
{
    ASSERT_EQ(max(1, 2), 2);
    ASSERT_EQ(max(1, 10), 10);
    ASSERT_EQ(max(429, 0), 429);
    ASSERT_EQ(max(2, -3), 2);
}

TEST(MathAbsTest, Zero)
{
    ASSERT_EQ(abs(0), 0);
}

TEST(MathAbsTest, Positive)
{
    ASSERT_EQ(abs(5), 5);
    ASSERT_EQ(abs(1), 1);
    ASSERT_EQ(abs(256), 256);
    ASSERT_EQ(abs(19442), 19442);
}

TEST(MathAbsTest, Negative)
{
    ASSERT_EQ(abs(-4), 4);
    ASSERT_EQ(abs(-229), 229);
    ASSERT_EQ(abs(-64), 64);
    ASSERT_EQ(abs(-256), 256);
    ASSERT_EQ(abs(-65536), 65536);
}
