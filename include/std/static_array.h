#ifndef STD_STATIC_ARRAY_H
#define STD_STATIC_ARRAY_H

#include <std/base_array.h>

template <typename T, size_t N>
class StaticArray : public BaseArray<T, StaticArray<T, N>>
{
public:
    using Iterator = ArrayIterator<T>;
    using ConstIterator = ArrayIterator<const T>;
    using ReverseIterator = ::ReverseIterator<Iterator>;
    using ConstReverseIterator = ::ReverseIterator<ConstIterator>;

    StaticArray()
    {
        for (size_t i = 0; i < N; ++i)
            _data[N] = T();
    }

    explicit StaticArray(const T& value)
    {
        for (size_t i = 0; i < N; ++i)
            _data[N] = value;
    }

    StaticArray(const StaticArray<T, N>& other)
    {
        for (size_t i  = 0; i < N; ++i)
            _data[N] = other._data[N];
    }

    StaticArray<T, N>& operator=(const StaticArray<T, N>& other)
    {
        for (size_t i = 0; i < N; ++i)
            _data[N] = other._data[N];
    }

    ~StaticArray()
    {

    }

    size_t size() const
    {
        return N;
    }

    size_t capacity() const
    {
        return N;
    }

    const T* data() const
    {
        return _data;
    }

    T* data()
    {
        return _data;
    }

private:
    T   _data[N];
};

#endif
