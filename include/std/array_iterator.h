#ifndef STD_ARRAY_ITERATOR_H
#define STD_ARRAY_ITERATOR_H

#include <cstddef>

template <typename T>
class ArrayIterator
{
public:
    using ValueType = T;

    ArrayIterator()
    : _ptr(nullptr)
    {

    }

    explicit ArrayIterator(T* ptr)
    : _ptr(ptr)
    {

    }

    ArrayIterator(const ArrayIterator<T>& other)
    : _ptr(other._ptr)
    {

    }

    ArrayIterator<T>& operator=(const ArrayIterator<T>& other)
    {
        _ptr = other._ptr;
        return *this;
    }

    ~ArrayIterator()
    {

    }

    constexpr bool operator==(const ArrayIterator<T>& rhs) const
    {
        return _ptr == rhs._ptr;
    }

    constexpr bool operator!=(const ArrayIterator<T>& rhs) const
    {
        return !(*this == rhs);
    }

    constexpr bool operator<(const ArrayIterator<T>& rhs) const
    {
        return _ptr < rhs._ptr;
    }

    constexpr bool operator<=(const ArrayIterator<T>& rhs) const
    {
        return *this == rhs || *this < rhs;
    }

    constexpr bool operator>(const ArrayIterator<T>& rhs) const
    {
        return !(*this <= rhs);
    }

    constexpr bool operator>=(const ArrayIterator<T>& rhs) const
    {
        return !(*this < rhs);
    }

    T& operator*() const
    {
        return *_ptr;
    }

    T* operator->() const
    {
        return _ptr;
    }

    T& operator[](ptrdiff_t i) const
    {
        return _ptr[i];
    }

    ArrayIterator<T> operator++()
    {
        ++_ptr;
        return *this;
    }

    ArrayIterator<T> operator++(int)
    {
        ArrayIterator<T> tmp(*this);
        ++_ptr;
        return tmp;
    }

    ArrayIterator<T> operator--()
    {
        --_ptr;
        return *this;
    }

    ArrayIterator<T> operator--(int)
    {
        ArrayIterator<T> tmp(*this);
        --_ptr;
        return tmp;
    }

    ArrayIterator<T>& operator+=(ptrdiff_t n)
    {
        _ptr += n;
        return *this;
    }

    ArrayIterator<T>& operator-=(ptrdiff_t n)
    {
        _ptr -= n;
        return *this;
    }

private:
    T* _ptr;
};

template <typename T>
ArrayIterator<T> operator+(ArrayIterator<T> it, ptrdiff_t n)
{
    return it += n;
}

template <typename T>
ArrayIterator<T> operator-(ArrayIterator<T> it, ptrdiff_t n)
{
    return it -= n;
}

template <typename T>
void swap(ArrayIterator<T> a, ArrayIterator<T> b)
{
    T tmp(*a);
    *a = *b;
    *b = tmp;
}

#endif
