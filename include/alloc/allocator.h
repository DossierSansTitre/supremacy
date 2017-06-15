#ifndef ALLOC_ALLOCATOR_H
#define ALLOC_ALLOCATOR_H

#include <cstdlib>

template <typename T>
class Allocator
{
public:
    Allocator() {}
    Allocator(const Allocator& rhs) {}
    ~Allocator() {}

    Allocator<T>& operator=(const Allocator& rhs) { return *this; }

    T* allocate()
    {
        return allocate_array(1);
    }

    T* allocate_array(size_t n)
    {
        void* ptr;

        ptr = malloc(sizeof(T) * n);
        return reinterpret_cast<T*>(ptr);
    }

    void destroy(T* ptr)
    {
        destroy_array(ptr, 1);
    }

    void destroy_array(T* ptr, size_t n)
    {
        (void)n;
        free(ptr);
    }
};

#endif
