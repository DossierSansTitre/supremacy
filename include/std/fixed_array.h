#ifndef STD_FIXED_ARRAY_H
#define STD_FIXED_ARRAY_H

#include <math/algebra.h>
#include <std/array_iterator.h>
#include <std/reverse_iterator.h>

template <typename T, size_t N>
class FixedArray
{
public:
    using Iterator = ArrayIterator<T>;
    using ConstIterator = ArrayIterator<const T>;
    using ReverseIterator = ::ReverseIterator<Iterator>;
    using ConstReverseIterator = ::ReverseIterator<ConstIterator>;

    FixedArray()
    : _size(0)
    {

    }

    explicit FixedArray(size_t size)
    : _size(size)
    {
        for (size_t i = 0; i < size; ++i)
            new (_data + i * sizeof(T)) T;
    }

    FixedArray(size_t size, const T& value)
    : _size(size)
    {
        for (size_t i = 0; i < size; ++i)
            new (_data + i * sizeof(T)) T(value);
    }

    FixedArray(const FixedArray<T, N>& other)
    : _size(other._size)
    {
        for (size_t i = 0; i < _size; ++i)
            new (_data + i * sizeof(T)) T(*reinterpret_cast<T*>(other._data + i * sizeof(T)));
    }

    FixedArray<T, N>& operator=(const FixedArray<T, N>& other)
    {
        size_t other_size = other._size;
        size_t min_size = min(_size, other_size);

        for (size_t i = 0; i < min_size; ++i)
            *reinterpret_cast<T*>(_data + i * sizeof(T)) = *reinterpret_cast<T*>(other._data + i * sizeof(T));

        if (_size < other_size)
        {
            for (size_t i = _size; i < other_size; ++i)
                new (_data + i * sizeof(T)) T(*reinterpret_cast<T*>(other._data + i * sizeof(T)));
        }
        else if (_size > other_size)
        {
            for (size_t i = other_size; i < _size; ++i)
                reinterpret_cast<T*>(_data + i * sizeof(T))->~T();
        }
        _size = other_size;
        return *this;
    }

    ~FixedArray()
    {
        for (size_t i = 0; i < _size; ++i)
            reinterpret_cast<T*>(_data + i * sizeof(T))->~T();
    }

    bool empty() const
    {
        return _size == 0;
    }

    bool full() const
    {
        return _size == N;
    }

    size_t size() const
    {
        return _size;
    }

    size_t capacity() const
    {
        return N;
    }

    const T* data() const
    {
        return reinterpret_cast<const T*>(_data);
    }

    T* data()
    {
        return reinterpret_cast<T*>(_data);
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
        return data()[_size - 1];
    }

    T& back()
    {
        return data()[_size - 1];
    }

    void push_front(const T& value)
    {
        for (size_t i = 0; i < _size; ++i)
            data()[_size - i] = data()[_size - i - 1];
        new (data()) T(value);
        _size++;
    }

    void push_back(const T& value)
    {
        new (data() + _size) T(value);
        _size++;
    }

    void pop_front()
    {
        data()->~T();
        for (size_t i = 0; i < _size - 1; ++i)
            _data()[i] = _data()[i + 1];
        _size--;
    }

    void pop_back()
    {
        (data() + (_size - 1))->~T();
        _size--;
    }

    void resize(size_t size)
    {
        if (size < _size)
        {
            for (size_t i = size; i < _size; ++i)
                (data() + i)->~T();
        }
        else
        {
            for (size_t i = _size; i < size; ++i)
                new (data() + i) T;
        }
        _size = size;
    }

    Iterator begin()
    {
        return Iterator(data());
    }

    ConstIterator begin() const
    {
        return ConstIterator(data());
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
        return Iterator(data() + _size);
    }

    ConstIterator end() const
    {
        return ConstIterator(data() + _size);
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
    size_t          _size;
    alignas(T) char _data[sizeof(T) * N];
};

#endif
