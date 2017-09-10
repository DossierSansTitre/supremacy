#include <worldmap.h>

Worldmap::Worldmap(u16 id, Vector2i size)
: _id(id)
, _size(size)
{
    size_t s;

    s = size.x * size.y;
    _biomes.resize(s);
    _height.resize(s);
    _temperature.resize(s);
    _rain.resize(s);
    _drainage.resize(s);
}
