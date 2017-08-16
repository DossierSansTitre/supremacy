#include <worldmap.h>

Worldmap::Worldmap(Vector2i size)
: _size(size)
{
    _biomes.resize(size.x * size.y);
}

void Worldmap::set_biome(Vector2i pos, BiomeID biome)
{
    _biomes[pos.x + pos.y * _size.x] = biome;
}
