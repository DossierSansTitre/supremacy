#ifndef MATH_RECT_H
#define MATH_RECT_H

#include <math/vector.h>

template <typename T, size_t N>
class Rect
{
public:
    using VectorType = Vector<T, N>;

    Rect()
    {

    }

    Rect(VectorType origin, VectorType size)
    : origin(origin)
    , size(size)
    {

    }

    Rect(const Rect& other)
    : origin(other.origin)
    , size(other.size)
    {

    }

    ~Rect()
    {

    }

    Rect<T, N>& operator=(const Rect& other)
    {
        origin = other.origin;
        size = other.size;
        return *this;
    }

    VectorType  origin;
    VectorType  size;
};

template <typename T> using Rect2 = Rect<T, 2>;
template <typename T> using Rect3 = Rect<T, 3>;
template <typename T> using Rect4 = Rect<T, 4>;

using Rect2i = Rect2<int32_t>;
using Rect3i = Rect3<int32_t>;
using Rect4i = Rect4<int32_t>;

using Rect2u = Rect2<uint32_t>;
using Rect3u = Rect3<uint32_t>;
using Rect4u = Rect4<uint32_t>;

#endif
