#ifndef MEMORY_FILE_H
#define MEMORY_FILE_H

#include <cstdint>
#include <cstdlib>

class MemoryFile
{
public:
    MemoryFile();

    size_t size() const { return _size; }
    size_t tell() const { return _cursor; }

    void open(void* ptr, size_t size);
    void close();

    bool read(void* dst, size_t len);

    template <typename T>
    bool read(T* ptr)
    {
        return read(ptr, sizeof(T));
    }

private:
    void*   _data;
    size_t  _size;
    size_t  _cursor;
};

#endif
