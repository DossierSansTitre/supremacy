#ifndef SUP_FILE_H
#define SUP_FILE_H

#include <cstdint>
#include <cstdlib>
#include <non_copyable.h>
#include <std/array.h>

class Archive;
class MemoryFile;

class SupFile : private NonCopyable
{
public:
    SupFile();
    ~SupFile();

    void open(const char* data, size_t size);
    void open(Archive& archive, const char* path);

    bool read(MemoryFile& file);

    void close();

private:
    char*           _data;
    uint32_t        _count;
    uint32_t        _cursor;
    Array<uint32_t> _offsets;
    Array<uint32_t> _sizes;
    size_t          _header_size;
};

#endif
