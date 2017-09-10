#ifndef MATH_ALGORITHM_H
#define MATH_ALGORITHM_H

#include <math/vector.h>

template <typename T, typename F>
void iterate(Vector<T, 3> size, const F& fn)
{
    for (T z = T(); z < size.z; ++z)
    {
        for (T y = T(); y < size.y; ++y)
        {
            for (T x = T(); x < size.x; ++x)
            {
                fn(Vector<T, 3>(x, y, z));
            }
        }
    }
}

template <typename T, typename F>
void iterate(Vector<T, 2> size, const F& fn)
{
    for (T y = T(); y < size.y; ++y)
    {
        for (T x = T(); x < size.x; ++x)
        {
            fn(Vector<T, 2>(x, y));
        }
    }
}

template <class T>
constexpr const T& min(const T& a, const T& b)
{
    return (a < b) ? a : b;
}

template <class T>
constexpr const T& max(const T& a, const T& b)
{
    return (a < b) ? b : a;
}

#endif
