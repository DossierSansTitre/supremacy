#include <gtest/gtest.h>
#include <math/bitwise.h>

TEST(MathRot, HandlesLeftRot)
{
    ASSERT_EQ(rotl8(0x10, 1), 0x20);
    ASSERT_EQ(rotl8(0xf0, 4), 0x0f);
    ASSERT_EQ(rotl8(0x81, 1), 0x03);
    ASSERT_EQ(rotl64(0xf000000000000001, 4), 0x1f);
}

TEST(MathRot, HandlesRightRot)
{
    ASSERT_EQ(rotr8(0x01, 1), 0x80);
    ASSERT_EQ(rotr8(0xf0, 4), 0x0f);
    ASSERT_EQ(rotr8(0x11, 2), 0x44);
    ASSERT_EQ(rotr64(0xf000000000000001, 4), 0x1f00000000000000);
}
