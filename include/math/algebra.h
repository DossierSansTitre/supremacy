#ifndef MATH_ALGEBRA_H
#define MATH_ALGEBRA_H

template <typename T>
T min(T a, T b)
{
    return a < b ? a : b;
}

template <typename T>
T max(T a, T b)
{
    return a < b ? b : a;
}

template <typename T>
T abs(T value)
{
    return value < T() ? -value : value;
}

#endif
