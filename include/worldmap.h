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
    size_t      index(Vector2i pos) const { return pos.x + _size.x * pos.y; }
    Vector2i    size() const { return _size; }
    BiomeID     biome(Vector2i pos) const { return _biomes[index(pos)]; }
    u16         height(Vector2i pos) const { return _height[index(pos)]; }
    u16         temperature(Vector2i pos) const { return _temperature[index(pos)]; }
    u16         rain(Vector2i pos) const { return _rain[index(pos)]; }
    u16         drainage(Vector2i pos) const { return _drainage[index(pos)]; }

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
