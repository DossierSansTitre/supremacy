#ifndef SPARSE_ARRAY_H
#define SPARSE_ARRAY_H

#include <std/array.h>

template <typename T, typename Alloc = Allocator<T>, typename IndiceAlloc = Allocator<uint32_t>>
class SparseArray;

template <typename T>
class SparseArrayIterator
{
public:
    SparseArrayIterator()
    : _key(nullptr)
    , _value(nullptr)
    {

    }

    SparseArrayIterator(uint32_t* key, T* value)
    : _key(key)
    , _value(value)
    {

    }

    bool operator==(const SparseArrayIterator<T>& rhs) const
    {
        return (_key == other._key);
    }

    bool operator!=(const SparseArrayIterator<T>& rhs) const
    {
        return !(*this == rhs);
    }



private:
    uint32_t*   _key;
    T*          _value;
}

template <typename T, typename Alloc = Allocator<T>, typename IndiceAlloc = Allocator<uint32_t>>
class SparseArray
{
public:
    size_t size() const
    {
        return _data.size();
    }

    size_t index_size() const
    {
        return _external.size();
    }

    T& fetch_or_create(uint32_t i)
    {
        uint32_t internal;

        if (i >= _external.size())
            _external.resize(i + 1);
        internal = _external[i];
        if (internal == 0)
        {
            internal = _data.size() + 1;
            _data.resize(internal);
            _external[i] = internal;
            _reverse.push_back(i);
        }
        return _data[internal - 1];
    }

    void append(uint32_t i, const T& value)
    {
        fetch_or_create(i) = value;
    }

    const T& operator[](uint32_t i) const
    {
        return _data[_external[i] - 1];
    }

    T& operator[](uint32_t i)
    {
        return _data[_external[i] - 1];
    }

    bool has(uint32_t i) const
    {
        return (_external[i] != 0);
    }

    void remove(uint32_t i)
    {
        uint32_t internal;

        internal = _external[i];
        if (internal == 0)
            return;
        if (internal != _data.size())
        {
            swap(_data[internal - 1], _data.back());
            swap(_reverse[internal - 1], _reverse.back());
        }
        _data.pop_back();
        _reverse.pop_back();
    }

    uint32_t key(size_t i) const
    {
        return _reverse[i];
    }

    const T& value(size_t i) const
    {
        return _data[i];
    }

    T& value(size_t i)
    {
        return _data[i];
    }

private:
    Array<uint32_t, IndiceAlloc>    _external;
    Array<uint32_t, IndiceAlloc>    _reverse;
    Array<T, Alloc>                 _data;
};

#endif
