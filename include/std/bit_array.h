#ifndef STD_BIT_ARRAY_H
#define STD_BIT_ARRAY_H

#include <std/array.h>
#include <cstdint>
#include <log.h>

template <typename T>
class BitArrayReference
{
public:
    BitArrayReference(T* ptr, size_t bits)
    : _ptr(ptr)
    , _bits(bits)
    {

    }

    operator bool() const
    {
        return ((*_ptr) & (T(1) << _bits));
    }

    BitArrayReference& operator=(bool value)
    {
        if (value)
            (*_ptr) |= (T(1) << _bits);
        else
            (*_ptr) &= ~(T(1) << _bits);
        return *this;
    }

private:
    T*          _ptr;
    size_t      _bits;
};

template <typename T = size_t, typename Alloc = Allocator<T>>
class BitArray
{
public:
    static const uint32_t bits_per_chunk = sizeof(T) * 8;

    BitArray()
    : _size(0u)
    {

    }

    explicit BitArray(size_t size)
    : _size(0)
    {
        resize(size);
    }

    BitArray(const BitArray& other)
    : _data(other._data)
    , _size(other._size)
    {

    }

    BitArray(BitArray&& other)
    : _size(other._size)
    {
        other._size = 0u;
        swap(_data, other._data);
    }

    BitArray& operator=(const BitArray& other)
    {
        _size = other._size;
        _data = other._data;
        return *this;
    }

    BitArray& operator=(BitArray&& other)
    {
        swap(_size, other._size);
        swap(_data, other._data);
        return *this;
    }

    ~BitArray()
    {

    }

    size_t size() const
    {
        return _size;
    }

    size_t capacity() const
    {
        return _data.capacity() * bits_per_chunk;
    }

    bool empty() const
    {
        return size() == 0u;
    }

    const char* data() const
    {
        return reinterpret_cast<const char*>(_data.data());
    }

    char* data()
    {
        return reinterpret_cast<char*>(_data.data());
    }

    void push_back(bool value)
    {
        size_t bit_position;

        bit_position = _size % bits_per_chunk;
        if (bit_position == 0)
            _data.push_back(T(value));
        else if (value)
            _data.back() |= (T(value) << bit_position);
        _size++;
    }

    void pop_back()
    {
        _size--;
    }

    bool operator[](size_t i) const
    {
        return _data[i / bits_per_chunk] & (T(1) << (i % bits_per_chunk));
    }

    BitArrayReference<T> operator[](size_t i)
    {
        return BitArrayReference<T>(_data.data() + (i / bits_per_chunk), i % bits_per_chunk);
    }

    void resize(size_t size)
    {
        size_t container_size;

        container_size = (size + (bits_per_chunk - 1)) / bits_per_chunk;
        _data.resize(container_size);
        _size = size;
    }

private:
    Array<T, Alloc> _data;
    size_t          _size;
};

#endif
