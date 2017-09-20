#ifndef STD_REVERSE_ITERATOR_H
#define STD_REVERSE_ITERATOR_H

#include <cstddef>
#include <std/algorithm.h>

template <typename Iter>
class ReverseIterator
{
public:
    ReverseIterator()
    {

    }

    explicit ReverseIterator(Iter iter)
    : _iter(iter)
    {

    }

    ReverseIterator(const ReverseIterator<Iter>& other)
    : _iter(other._iter)
    {

    }

    ReverseIterator<Iter>& operator=(const ReverseIterator<Iter>& other)
    {
        _iter = other._iter;
    }

    ~ReverseIterator()
    {

    }

    constexpr bool operator==(const ReverseIterator<Iter>& other) const
    {
        return _iter == other._iter;
    }

    constexpr bool operator!=(const ReverseIterator<Iter>& other) const
    {
        return !(*this == other);
    }

    constexpr bool operator<(const ReverseIterator<Iter>& other) const
    {
        return _iter < other._iter;
    }

    constexpr bool operator<=(const ReverseIterator<Iter>& other) const
    {
        return *this == other || *this < other;
    }

    constexpr bool operator>(const ReverseIterator<Iter>& other) const
    {
        return !(*this <= other);
    }

    constexpr bool operator>=(const ReverseIterator<Iter>& other) const
    {
        return !(*this < other);
    }

    const Iter base() const
    {
        return _iter;
    }

    Iter base()
    {
        return _iter;
    }

    typename Iter::ValueType& operator*() const
    {
        Iter tmp(_iter);
        --tmp;
        return *tmp;
    }

    typename Iter::ValueType* operator->() const
    {
        return &(this->operator*());
    }

    typename Iter::ValueType& operator[](ptrdiff_t n) const
    {
        return _iter[n - 1];
    }

    ReverseIterator<Iter> operator++()
    {
        --_iter;
        return *this;
    }

    ReverseIterator<Iter> operator++(int)
    {
        ReverseIterator<Iter> tmp(*this);
        --_iter;
        return tmp;
    }

    ReverseIterator<Iter> operator--()
    {
        ++_iter;
        return *this;
    }

    ReverseIterator<Iter> operator--(int)
    {
        ReverseIterator<Iter> tmp(*this);
        ++_iter;
        return tmp;
    }

    ReverseIterator<Iter>& operator+=(ptrdiff_t n)
    {
        _iter -= n;
        return *this;
    }

    ReverseIterator<Iter>& operator-=(ptrdiff_t n)
    {
        _iter += n;
        return *this;
    }

private:
    Iter    _iter;
};

template <typename Iter>
ReverseIterator<Iter> operator+(ReverseIterator<Iter> it, ptrdiff_t n)
{
    return it += n;
}

template <typename Iter>
ReverseIterator<Iter> operator-(ReverseIterator<Iter> it, ptrdiff_t n)
{
    return it -= n;
}

template <typename Iter>
ReverseIterator<Iter> reverse_iterator(const Iter& iter)
{
    return ReverseIterator<Iter>(iter);
}

template <typename Iter>
Iter reverse_iterator(const ReverseIterator<Iter>& iter)
{
    return iter.base();
}

template <typename Iter>
void swap(const ReverseIterator<Iter>& a, const ReverseIterator<Iter>& b)
{
    swap(a.base(), b.base());
}

#endif
