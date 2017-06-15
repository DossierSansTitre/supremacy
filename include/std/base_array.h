#ifndef STD_ARRAY_H
#define STD_ARRAY_H

#include <std/array_iterator.h>
#include <std/reverse_iterator.h>

template <typename T, typename Derived>
class BaseArray
{
public:
    size_t size() const
    {
        return static_cast<const Derived*>(this)->size();
    }

    const T* data() const
    {
        return static_cast<const Derived*>(this)->data(); 
    }

    T* data()
    {
        return static_cast<Derived*>(this)->data(); 
    }

    bool empty() const
    {
        return size() == 0u;
    }

    const T& operator[](size_t i) const
    {
        return data()[i];
    }

    T& operator[](size_t i)
    {
        return data()[i];
    }

    const T& front() const
    {
        return data()[0];
    }

    T& front()
    {
        return data()[0];
    }

    const T& back() const
    {
        return data()[size() - 1];
    }

    T& back()
    {
        return data()[size() - 1];
    }

    auto begin()
    {
        using Iterator = typename Derived::Iterator;
        return Iterator(data());
    }

    auto begin() const
    {
        using ConstIterator = typename Derived::ConstIterator;
        return ConstIterator(data());
    }

    auto cbegin() const
    {
        return begin();
    }

    auto rbegin()
    {
        return reverse_iterator(begin());
    }

    auto rbegin() const
    {
        return reverse_iterator(begin());
    }

    auto crbegin() const
    {
        return rbegin();
    }

    auto end()
    {
        using Iterator = typename Derived::Iterator;
        return Iterator(data() + size());
    }

    auto end() const
    {
        using ConstIterator = typename Derived::ConstIterator;
        return ConstIterator(data() + size());
    }

    auto cend() const
    {
        return end();
    }

    auto rend()
    {
        return reverse_iterator(end());
    }

    auto rend() const
    {
        return reverse_iterator(end());
    }

    auto crend() const
    {
        return rend();
    }
};

#endif
