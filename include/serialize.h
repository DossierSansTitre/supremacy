#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <archive.h>
#include <std/sparse_array.h>
#include <log.h>

template <typename T, typename Func>
void unserialize_resource_array(SparseArray<T>& array, Archive& archive, const char* filename, Func func)
{
    SupFile sup;
    MemoryFile file;
    int count = 0;

    sup.open(archive, filename);
    while (sup.read(file))
    {
        uint16_t id;

        file.read(&id);
        func(array.fetch_or_create(id), file);
        count++;
    }
    log_line(LogLevel::Info, "Loaded %-20s (%d elements)", filename, count);
}

#endif
