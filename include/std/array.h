#ifndef STD_ARRAY_H
#define STD_ARRAY_H

#include <alloc/allocator.h>

#include <std/array_iterator.h>
#include <std/reverse_iterator.h>

template <typename T, typename Alloc = Allocator<T>>
class Array
{
public:
    Array()
    : _size(0)
    , _capacity(0)
    , _data(nullptr)
    {

    }

    explicit Array(size_t size)
    : Array()
    {
        resize(size);
    }

    Array(const Array<T>& other)
    : _size(other._size)
    , _capacity(other._capacity)
    , 

private:
    size_t  _size;
    size_t  _capacity;
    T*      _data;
    Alloc   _alloc;
};

#endif
