#ifndef WORLDMAP_H
#define WORLDMAP_H

#include <types.h>
#include <std/array.h>
#include <math/vector.h>

class Worldmap
{
public:
    Worldmap(Vector2i size);

    Vector2i    size() const { return _size; }
    BiomeID     biome(Vector2i pos) const { return _biomes[pos.x + pos.y * _size.x]; }

    void        set_biome(Vector2i pos, BiomeID biome);

private:
    Vector2i        _size;
    Array<BiomeID>  _biomes;
};

#endif
