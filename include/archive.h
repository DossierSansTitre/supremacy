#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <fstream>
#include <non_copyable.h>
#include <std/array.h>
#include <memory_file.h>
#include <sup_file.h>

class Archive : private NonCopyable
{
public:
    Archive();
    ~Archive();

    void    open(const char* path);
    bool    read(char** data, uint32_t* size, const char* file);
    void    close();

private:
    bool            _open;
    std::ifstream   _stream;
    uint32_t        _count;
    Array<char*>    _names;
    Array<uint32_t> _offsets;
    Array<uint32_t> _sizes;
    size_t          _header_size;
};

#endif
