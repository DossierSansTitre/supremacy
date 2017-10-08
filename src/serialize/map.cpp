#include <serialize.h>
#include <map.h>

void serialize_map(std::ofstream& stream, const Map& map)
{
    size_t size;

    stream.write((char*)&map._width, sizeof(int));
    stream.write((char*)&map._height, sizeof(int));
    stream.write((char*)&map._depth, sizeof(int));

    size = map._width * map._height * map._depth;

    stream.write((char*)map._tiles, sizeof(TileID) * size);
    stream.write((char*)map._materials, sizeof(MaterialID) * size);
    stream.write((char*)map._floors.data(), sizeof(MaterialID) * size);
}

void unserialize_map(Map& map, std::ifstream& stream)
{
    size_t size;
    int width;
    int height;
    int depth;

    stream.read((char*)&width, sizeof(int));
    stream.read((char*)&height, sizeof(int));
    stream.read((char*)&depth, sizeof(int));

    size = width * height * depth;

    map.create(width, height, depth);
    stream.read((char*)map._tiles, sizeof(TileID) * size);
    stream.read((char*)map._materials, sizeof(MaterialID) * size);
    stream.read((char*)map._floors.data(), sizeof(MaterialID) * size);
}
