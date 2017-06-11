#ifndef STD_STATIC_ARRAY_H
#define STD_STATIC_ARRAY_H

#include <std/array_iterator.h>
#include <std/reverse_iterator.h>

template <typename T, size_t N>
class StaticArray
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

    const T& operator[](size_t i) const
    {
        return _data[i];
    }

    T& operator[](size_t i)
    {
        return _data[i];
    }

    Iterator begin()
    {
        return Iterator(_data);
    }

    ConstIterator begin() const
    {
        return ConstIterator(_data);
    }

    ConstIterator cbegin() const
    {
        return begin();
    }

    ReverseIterator rbegin()
    {
        return reverse_iterator(begin());
    }

    ConstReverseIterator rbegin() const
    {
        return reverse_iterator(begin());
    }

    ConstReverseIterator crbegin() const
    {
        return rbegin();
    }

    Iterator end()
    {
        return Iterator(_data + N);
    }

    ConstIterator end() const
    {
        return ConstIterator(_data + N);
    }

    ConstIterator cend() const
    {
        return end();
    }

    ReverseIterator rend()
    {
        return reverse_iterator(end());
    }

    ConstReverseIterator rend() const
    {
        return reverse_iterator(end());
    }

    ConstReverseIterator crend() const
    {
        return rend();
    }

private:
    T   _data[N];
};

#endif
