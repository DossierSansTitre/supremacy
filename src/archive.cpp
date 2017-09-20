#include <cstring>
#include <archive.h>

Archive::Archive()
: _open(false)
{

}

Archive::~Archive()
{
    close();
}

void Archive::open(const char* path)
{
    char supa[4];

    close();
    _stream.open(path, std::ios::binary);
    _stream.read(supa, 4);
    _stream.read(reinterpret_cast<char*>(&_count), 4);
    _names.resize(_count);
    _sizes.resize(_count);
    _offsets.resize(_count);

    for (uint32_t i = 0; i < _count; ++i)
    {
        uint16_t name_len;
        char* name;

        _stream.read(reinterpret_cast<char*>(&name_len), 2);
        name = new char[name_len + 1];
        _stream.read(name, name_len);
        name[name_len] = 0;
        _names[i] = name;
        _stream.read(reinterpret_cast<char*>(_offsets.data() + i), 4);
        _stream.read(reinterpret_cast<char*>(_sizes.data() + i), 4);
    }
    _header_size = _stream.tellg();
    _open = true;
}

bool Archive::read(char** data, uint32_t* size, const char* file)
{
    char* buffer;

    for (uint32_t i = 0; i < _count; ++i)
    {
        if (strcmp(_names[i], file) == 0)
        {
            buffer = new char[_sizes[i]];
            _stream.seekg(_header_size + _offsets[i], _stream.beg);
            _stream.read(buffer, _sizes[i]);
            *data = buffer;
            *size = _sizes[i];
            return true;
        }
    }
    return false;
}

void Archive::close()
{
    _stream.close();
}
