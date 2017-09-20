#include <cstring>
#include <sup_file.h>
#include <memory_file.h>
#include <archive.h>

SupFile::SupFile()
: _data(nullptr)
, _count(0)
, _cursor(0)
, _header_size(0)
{

}

SupFile::~SupFile()
{
    close();
}

void SupFile::open(const char* data, size_t size)
{
    char sup[4];
    MemoryFile file;

    close();
    _data = new char[size];
    std::memcpy(_data, data, size);
    file.open(_data, size);
    file.read(sup, 4);
    file.read(&_count);

    _offsets.resize(_count);
    _sizes.resize(_count);
    for (uint32_t i = 0; i < _count; ++i)
    {
        file.read(_offsets.data() + i);
        file.read(_sizes.data() + i);
    }
    _header_size = file.tell();
}

void SupFile::open(Archive& archive, const char* path)
{
    char* data;
    uint32_t size;

    close();

    if (archive.read(&data, &size, path))
    {
        open(data, size);
        delete [] data;
    }
}

bool SupFile::read(MemoryFile& file)
{
    if (_cursor >= _count)
        return false;
    file.open(_data + _header_size + _offsets[_cursor], _sizes[_cursor]);
    _cursor++;
    return true;
}

void SupFile::close()
{
    delete [] _data;
    _data = nullptr;
    _count = 0;
    _cursor = 0;
    _offsets.resize(0);
    _sizes.resize(0);
    _header_size = 0;
}
