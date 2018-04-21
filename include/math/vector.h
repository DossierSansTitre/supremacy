#ifndef MATH_VECTOR_H
#define MATH_VECTOR_H

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace priv
{

template <typename T, size_t N>
struct VectorStorage
{
    union {
        T   _data[N];
        struct {
            T   x;
            T   y;
            T   z;
            T   w;
        };
    };
};

template <typename T>
struct VectorStorage<T, 1>
{
    union {
        T   _data[1];
        struct {
            T   x;
        };
    };
};

template <typename T>
struct VectorStorage<T, 2>
{
    union {
        T   _data[2];
        struct {
            T   x;
            T   y;
        };
    };
};

template <typename T>
struct VectorStorage<T, 3>
{
    union {
        T   _data[3];
        struct {
            T   x;
            T   y;
            T   z;
        };
    };
};

template <typename T, size_t N, size_t Count>
struct VectorBuilder
{
    template <typename... Args>
    static void build(T* dst, const Args& ... args)
    {
        static_assert(Count >= N, "Missing elements in vector");
        static_assert(Count <= N, "Extra elements in vector");

        (void)dst;
        (void)(sizeof...(args));
    }

    template <typename TT, typename... Args>
    static typename std::enable_if<std::is_convertible<TT, T>::value>::type build(T* dst, TT value, const Args& ... args)
    {
        static_assert(Count < N, "Extra elements in vector");

        dst[Count] = (T)value;
        ::priv::VectorBuilder<T, N, Count + 1>::build(dst, args...);
    }
};

};

template <typename T, size_t N>
class Vector : public priv::VectorStorage<T, N>
{
public:
    using Type = T;
    static const size_t Size = N;

    static_assert(N > 0, "Must have non-zero size");

    Vector()
    {
        for (size_t i = 0; i < N; ++i)
            this->_data[i] = T();
    }

    template <typename Head, typename... Args>
    Vector(const Head& head, const Args&... args)
    {
        priv::VectorBuilder<T, N, 0>::build(this->_data, head, args...);
    }

    template <typename TT, size_t NN>
    explicit Vector(const Vector<TT, NN>& rhs)
    {
        const size_t max = N < NN ? N : NN;

        for (size_t i = 0; i < max; ++i)
            this->_data[i] = T(rhs._data[i]);
        for (size_t i = max; i < N; ++i)
            this->_data[i] = T();
    }

    template <typename TT, size_t NN, typename Head, typename... Args>
    Vector(const Vector<TT, NN>& rhs, const Head& head, const Args&... args)
    {
        static_assert(NN < N, "Extra elements in vector");

        for (size_t i = 0; i < NN; ++i)
            this->_data[i] = T(rhs._data[i]);
        ::priv::VectorBuilder<T, N, NN>::build(this->_data, head, args...);
    }


    Vector<T, N>& operator=(const Vector<T, N>& rhs)
    {
        for (size_t i = 0; i < N; ++i)
            this->_data[i] = rhs._data[i];
        return *this;
    }

    ~Vector()
    {

    }

    const T* data() const
    {
        return this->_data;
    }

    T* data()
    {
        return this->_data;
    }

    const T& operator[](size_t i) const
    {
        return this->_data[i];
    }

    T& operator[](size_t i)
    {
        return this->_data[i];
    }

    Vector<T, N>& operator+=(Vector<T, N> rhs)
    {
        for (size_t i = 0; i < N; ++i)
            this->_data[i] += rhs._data[i];
        return *this;
    }

    Vector<T, N>& operator-=(Vector<T, N> rhs)
    {
        for (size_t i = 0; i < N; ++i)
            this->_data[i] -= rhs._data[i];
        return *this;
    }

    Vector<T, N>& operator*=(Vector<T, N> rhs)
    {
        for (size_t i = 0; i < N; ++i)
            this->_data[i] *= rhs._data[i];
        return *this;
    }

    Vector<T, N>& operator/=(Vector<T, N> rhs)
    {
        for (size_t i = 0; i < N; ++i)
            this->_data[i] /= rhs._data[i];
        return *this;
    }

    Vector<T, N>& operator%=(Vector<T, N> rhs)
    {
        for (size_t i = 0; i < N; ++i)
            this->_data[i] %= rhs._data[i];
        return *this;
    }

    template <typename TT>
    Vector<T, N>& operator+=(TT scalar)
    {
        for (size_t i = 0; i < N; ++i)
            this->_data[i] += scalar;
        return *this;
    }

    template <typename TT>
    Vector<T, N>& operator-=(TT scalar)
    {
        for (size_t i = 0; i < N; ++i)
            this->_data[i] -= scalar;
        return *this;
    }

#pragma warning(push)
#pragma warning(disable: 4244)
    template <typename TT>
    Vector<T, N>& operator*=(TT scalar)
    {
        for (size_t i = 0; i < N; ++i)
            this->_data[i] *= scalar;
        return *this;
    }
#pragma warning(pop)

    template <typename TT>
    Vector<T, N>& operator/=(TT scalar)
    {
        for (size_t i = 0; i < N; ++i)
            this->_data[i] /= scalar;
        return *this;
    }

    template <typename TT>
    Vector<T, N>& operator%=(TT scalar)
    {
        for (size_t i = 0; i < N; ++i)
            this->_data[i] %= scalar;
        return *this;
    }

};

template <typename T, size_t N>
inline bool operator==(Vector<T, N> lhs, Vector<T, N> rhs)
{
    for (size_t i = 0; i < N; ++i)
    {
        if (lhs[i] != rhs[i])
            return false;
    }
    return true;
}

template <typename T, size_t N>
inline bool operator!=(Vector<T, N> lhs, Vector<T, N> rhs)
{
    return !(lhs == rhs);
}

template <typename T, size_t N>
inline bool operator<(Vector<T, N> lhs, Vector<T, N> rhs)
{
    for (size_t i = 0; i < N; ++i)
    {
        if (lhs[i] != rhs[i])
            return lhs[i] < rhs[i];
    }
    return false;
}

template <typename T, size_t N>
inline bool operator<=(Vector<T, N> lhs, Vector<T, N> rhs)
{
    return (lhs == rhs) || (lhs < rhs);
}

template <typename T, size_t N>
inline bool operator>(Vector<T, N> lhs, Vector<T, N> rhs)
{
    return !(lhs <= rhs);
}

template <typename T, size_t N>
inline bool operator>=(Vector<T, N> lhs, Vector<T, N> rhs)
{
    return !(lhs < rhs);
}

template <typename T, size_t N>
inline bool operator+(Vector<T, N> src)
{
    Vector<T, N> v(src);

    for (size_t i = 0; i < N; ++i)
        v[i] = +(v[i]);
    return v;
}

template <typename T, size_t N>
inline bool operator-(Vector<T, N> src)
{
    Vector<T, N> v(src);

    for (size_t i = 0; i < N; ++i)
        v[i] = -(v[i]);
    return v;
}

template <typename T, size_t N>
inline Vector<T, N> operator+(Vector<T, N> lhs, Vector<T, N> rhs)
{
    return (lhs += rhs);
}

template <typename T, size_t N>
inline Vector<T, N> operator-(Vector<T, N> lhs, Vector<T, N> rhs)
{
    return (lhs -= rhs);
}

template <typename T, size_t N>
inline Vector<T, N> operator*(Vector<T, N> lhs, Vector<T, N> rhs)
{
    return (lhs *= rhs);
}

template <typename T, size_t N>
inline Vector<T, N> operator/(Vector<T, N> lhs, Vector<T, N> rhs)
{
    return (lhs /= rhs);
}

template <typename T, size_t N>
inline Vector<T, N> operator%(Vector<T, N> lhs, Vector<T, N> rhs)
{
    return (lhs %= rhs);
}

template <typename T, size_t N, typename TT>
inline Vector<T, N> operator+(Vector<T, N> lhs, TT scalar)
{
    return (lhs += scalar);
}

template <typename T, size_t N, typename TT>
inline Vector<T, N> operator-(Vector<T, N> lhs, TT scalar)
{
    return (lhs -= scalar);
}

template <typename T, size_t N, typename TT>
inline Vector<T, N> operator*(Vector<T, N> lhs, TT scalar)
{
    return (lhs *= scalar);
}

template <typename T, size_t N, typename TT>
inline Vector<T, N> operator/(Vector<T, N> lhs, TT scalar)
{
    return (lhs /= scalar);
}

template <typename T, size_t N, typename TT>
inline Vector<T, N> operator%(Vector<T, N> lhs, TT scalar)
{
    return (lhs %= scalar);
}

template <typename T, size_t N, typename TT>
inline Vector<T, N> operator*(TT scalar, Vector<T, N> rhs)
{
    Vector<T, N> tmp;

    for (size_t i = 0; i < N; ++i)
        tmp[i] = scalar * rhs;
    return tmp;
}

template <typename T> using Vector2 = Vector<T, 2>;
template <typename T> using Vector3 = Vector<T, 3>;
template <typename T> using Vector4 = Vector<T, 4>;

using Vector2i = Vector2<int32_t>;
using Vector3i = Vector3<int32_t>;
using Vector4i = Vector4<int32_t>;

using Vector2u = Vector2<uint32_t>;
using Vector3u = Vector3<uint32_t>;
using Vector4u = Vector4<uint32_t>;

using Vector2l = Vector2<int64_t>;
using Vector3l = Vector3<int64_t>;
using Vector4l = Vector4<int64_t>;

using Vector2ul = Vector2<uint64_t>;
using Vector3ul = Vector3<uint64_t>;
using Vector4ul = Vector4<uint64_t>;

using Vector2f = Vector2<float>;
using Vector3f = Vector3<float>;
using Vector4f = Vector4<float>;

#endif
