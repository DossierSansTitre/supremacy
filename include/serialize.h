#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <archive.h>

template <typename T, typename Func>
void unserialize_resource_array(Array<T>& array, Archive& archive, const char* filename, Func func)
{
    SupFile sup;
    MemoryFile file;

    sup.open(archive, filename);
    while (sup.read(file))
    {
        uint16_t id;

        file.read(&id);
        if (id >= array.size())
            array.resize(id + 1);
        T& data = array[id];
        func(data, file);
    }
}

#endif
