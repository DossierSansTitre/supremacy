#ifndef WORLDMAP_H
#define WORLDMAP_H

#include <types.h>
#include <std/array.h>
#include <math/vector.h>
#include <fstream>

class Worldmap
{
public:
    friend void         serialize_worldmap(std::ofstream& stream, const Worldmap& worldmap);
    friend Worldmap*    unserialize_worldmap(std::ifstream& stream, u16 world_id);
    friend class WorldmapGenerator;

    Worldmap(u16 id, Vector2i size);

    u16         id() const { return _id; }
    Vector2i    size() const { return _size; }
    BiomeID     biome(Vector2i pos) const { return _biomes[pos.x + pos.y * _size.x]; }

private:
    u16             _id;
    Vector2i        _size;
    Array<BiomeID>  _biomes;
    Array<u16>      _height;
    Array<u16>      _temperature;
    Array<u16>      _rain;
    Array<u16>      _drainage;
};

#endif
