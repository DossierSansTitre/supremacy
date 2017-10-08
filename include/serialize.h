#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <fstream>
#include <archive.h>
#include <std/sparse_array.h>
#include <log.h>
#include <types.h>

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

template <typename T>
void serialize(std::ofstream& stream, const T* data, size_t size)
{
    stream.write(reinterpret_cast<const char*>(data), size * sizeof(T));
}

template <typename T>
void unserialize(std::ifstream& stream, T* data, size_t size)
{
    stream.read(reinterpret_cast<char*>(data), size * sizeof(T));
}

class Worldmap;
void save_worldmap(const Worldmap& worldmap);
void serialize_worldmap(std::ofstream& stream, const Worldmap& worldmap);
Worldmap* load_worldmap(u16 world_id);
Worldmap* unserialize_worldmap(std::ifstream& stream, u16 world_id);

struct World;
void save_world(const World& world);
void serialize_world(std::ofstream& stream, const World& world);
World* load_world(u16 world_id, u32 region_id);
World* unserialize_world(std::ifstream& stream, u16 world_id, u32 region_id);

class Map;
void serialize_map(std::ofstream& stream, const Map& map);
void unserialize_map(Map& map, std::ifstream& stream);

class Actors;
void serialize_actors(std::ofstream& stream, const Actors& actors);
void unserialize_actors(Actors& actors, std::ifstream& stream);

class Items;
void serialize_items(std::ofstream& stream, const Items& items);
void unserialize_items(Items& items, std::ifstream& stream);

#endif
