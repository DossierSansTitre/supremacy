#include <worldmap.h>
#include <serialize.h>
#include <util/file_path.h>

void save_worldmap(const Worldmap& worldmap)
{
    std::ofstream file;
    auto id = worldmap.id();
    char buffer[4096];

    snprintf(buffer, 4096, "worlds/world_%05u", id);
    make_path(save_path(buffer));
    strcat(buffer, "/map.bin");
    file.open(save_path(buffer), std::ios::binary);

    serialize_worldmap(file, worldmap);
}

void serialize_worldmap(std::ofstream& stream, const Worldmap& worldmap)
{
    size_t len;

    len = worldmap._size.x * worldmap._size.y;
    stream.write((char*)&worldmap._size.x, 4);
    stream.write((char*)&worldmap._size.y, 4);
    stream.write((char*)worldmap._biomes.data(), len * sizeof(BiomeID));
}

Worldmap* load_worldmap(u16 world_id)
{
    std::ifstream stream;
    char buffer[4096];

    snprintf(buffer, 4096, "worlds/world_%05u/map.bin", world_id);
    stream.open(save_path(buffer), std::ios::binary);

    return unserialize_worldmap(stream, world_id);
}

Worldmap* unserialize_worldmap(std::ifstream& stream, u16 world_id)
{
    Worldmap* worldmap;
    Vector2i size;

    stream.read((char*)&size.x, 4);
    stream.read((char*)&size.y, 4);
    worldmap = new Worldmap(world_id, size);
    stream.read((char*)worldmap->_biomes.data(), size.x * size.y * sizeof(BiomeID));

    return worldmap;
}
