#ifndef MATH_LINEAR_H
#define MATH_LINEAR_H

#include <math/rect.h>
#include <math/algebra.h>

template <typename T, size_t N>
Rect<T, N> rect_from_points(Vector<T, N> a, Vector<T, N> b)
{
    Rect<T, N> rect;

    for (size_t i = 0; i < N; ++i)
    {
        if (a[i] < b[i])
        {
            rect.origin[i] = a[i];
            rect.size[i] = b[i] - a[i];
        }
        else
        {
            rect.origin[i] = b[i];
            rect.size[i] = a[i] - b[i];
        }
    }
    return rect;
}

template <typename T, size_t N>
T length2(Vector<T, N> vec)
{
    T acc = T();

    for (size_t i = 0; i < N; ++i)
        acc += vec[i] * vec[i];
    return acc;
}

template <typename T, size_t N>
T distance2(Vector<T, N> a, Vector<T, N> b)
{
    return length2(a - b);
}

template <typename T, size_t N>
T manhattan_length(Vector<T, N> vec)
{
    T acc = T();

    for (size_t i = 0; i < N; ++i)
        acc += abs(vec[i]);
    return acc;
}

template <typename T, size_t N>
T manhattan_distance(Vector<T, N> a, Vector<T, N> b)
{
    return manhattan_length(a - b);
}

#endif
