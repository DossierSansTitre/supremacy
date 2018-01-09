#ifndef STD_ARRAY_H
#define STD_ARRAY_H

#include <new>
#include <alloc/allocator.h>
#include <std/base_array.h>

template <typename T, typename Alloc = Allocator<T>>
class Array : public BaseArray<T, Array<T, Alloc>>
{
public:
    using Iterator = ArrayIterator<T>;
    using ConstIterator = ArrayIterator<const T>;
    using ReverseIterator = ::ReverseIterator<Iterator>;
    using ConstReverseIterator = ::ReverseIterator<ConstIterator>;

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

    Array(const Array<T, Alloc>& other)
    : _size(other._size)
    , _capacity(other._capacity)
    , _alloc(other._alloc)
    {
        if (_capacity)
        {
            _data = _alloc.allocate_array(_capacity);
            for (size_t i = 0; i < _size; ++i)
                new (_data + i) T(other._data[i]);
        }
        else
            _data = nullptr;
    }

    Array(Array<T, Alloc>&& other)
    : _size(other._size)
    , _capacity(other._capacity)
    , _data(other._data)
    , _alloc(other._alloc)
    {
        other._size = 0;
        other._capacity = 0;
        other._data = nullptr;
    }

    Array<T, Alloc>& operator=(const Array<T, Alloc>& other)
    {
        _size = other._size;
        _capacity = other._capacity;
        _alloc = other._alloc;

        if (_capacity)
        {
            _data = _alloc.allocate_array(_capacity);
            for (size_t i = 0; i < _size; ++i)
                new (_data + i) T(other._data[i]);
        }
        else
            _data = nullptr;
        return *this;
    }

    Array<T, Alloc>& operator=(Array<T, Alloc>&& other)
    {
        swap(_size, other._size);
        swap(_capacity, other._capacity);
        swap(_data, other._data);
        swap(_alloc, other._alloc);
    }

    ~Array()
    {
        for (size_t i = 0; i < _size; ++i)
            (_data + i)->~T();
        _alloc.destroy_array(_data, _capacity);
    }

    size_t size() const
    {
        return _size;
    }

    size_t capacity() const
    {
        return _capacity;
    }

    const T* data() const
    {
        return _data;
    }

    T* data()
    {
        return _data;
    }

    void push_back(const T& value)
    {
        if (_size == _capacity)
            reserve(_capacity + 1);
        new (_data + _size) T(value);
        _size++;
    }

    void push_front(const T& value)
    {
        if (_size == _capacity)
            reserve(_capacity + 1);
        for (size_t i = 0; i < _size; ++i)
        {
            new (_data + (_size - i)) T(_data[_size - i - 1]);
            (_data + (_size - i - 1))->~T();
        }
        new (_data) T(value);
        _size++;
    }

    void pop_back()
    {
        (_data + _size - 1)->~T();
        _size--;
    }

    void pop_front()
    {
        _data->~T();
        for (size_t i = 0; i < _size - 1; ++i)
        {
            new (_data + i) T(_data[i + 1]);
            (_data + i + 1)->~T();
        }
        _size--;
    }

    void resize(size_t new_size)
    {
        if (new_size > _capacity)
            reserve(new_size);
        if (new_size > _size)
        {
            for (size_t i = 0; i < new_size - _size; ++i)
                new (_data + _size + i) T();
        }
        else if (new_size < _size)
        {
            for (size_t i = 0; i < _size - new_size; ++i)
                (_data + new_size + i)->~T();
        }
        _size = new_size;
    }

    void reserve(size_t target_size)
    {
        size_t new_capacity;
        T* new_data;

        new_capacity = _capacity;
        if (new_capacity < 16)
            new_capacity = 16;
        while (new_capacity < target_size)
            new_capacity += new_capacity / 2;
        new_data = _alloc.allocate_array(new_capacity);
        if (_data)
        {
            for (size_t i = 0; i < _size; ++i)
            {
                new (new_data + i) T(_data[i]);
                (_data + i)->~T();
            }
            _alloc.destroy_array(_data, _capacity);
        }
        _capacity = new_capacity;
        _data = new_data;
    }

    void clear()
    {
        resize(0);
    }

private:
    size_t  _size;
    size_t  _capacity;
    T*      _data;
    Alloc   _alloc;
};

#endif
