#include <cstring>
#include <memory_file.h>

MemoryFile::MemoryFile()
: _data(nullptr)
, _size(0)
, _cursor(0)
{

}

void MemoryFile::open(void* ptr, size_t size)
{
    _data = ptr;
    _size = size;
    _cursor = 0;
}

void MemoryFile::close()
{
    open(nullptr, 0);
}

bool MemoryFile::read(void* dst, size_t len)
{
    if (_cursor + len > _size)
        return false;
    memcpy(dst, (char*)_data + _cursor, len);
    _cursor += len;
    return true;
}
