#include <algorithm>
#include <map.h>

Map::Map()
: _width(0)
, _height(0)
, _depth(0)
, _tiles(nullptr)
, _materials(nullptr)
{

}

Map::~Map()
{
    destroy();
}

void Map::create(int width, int height, int depth)
{
    size_t size;

    _width = width;
    _height = height;
    _depth = depth;

    size = width * height * depth;

    _tiles = new TileID[size];
    _materials = new MaterialID[size];
    _visible.resize(size);

    std::fill(_tiles, _tiles + size, TileID::None);
    std::fill(_materials, _materials + size, MaterialID::None);
}

void Map::destroy()
{
    delete [] _tiles;
    delete [] _materials;
    _width = 0;
    _height = 0;
    _depth = 0;
    _tiles = nullptr;
    _materials = nullptr;
    _visible.resize(0);
}

void Map::at(int x, int y, int z, TileID &tile_id, MaterialID &material_id) const
{
    int i = index(x, y, z);

    tile_id = tile_at(i);
    material_id = material_at(i);
}

TileID Map::tile_at(int i) const
{
    if (i == -1)
        return TileID::None;
    return _tiles[i];
}

TileID Map::tile_at(int x, int y, int z) const
{
    return tile_at(index(x, y, z));
}

MaterialID Map::material_at(int i) const
{
    if (i == -1)
        return MaterialID::None;
    return _materials[i];
}

MaterialID Map::material_at(int x, int y, int z) const
{
    return material_at(index(x, y, z));
}

bool Map::visible(int i) const
{
    if (i == -1)
        return false;
    return _visible[i];
}

bool Map::visible(int x, int y, int z) const
{
    return visible(index(x, y, z));
}

int Map::index(int x, int y, int z) const
{
    if (x < 0 || x >= _width || y < 0 || y >= _height || z < 0 || z >= _depth)
        return -1;
    return x + y * _width + z * _width * _height;
}

void Map::set_tile(int index, TileID tile)
{
    if (index == -1)
        return;
    _tiles[index] = tile;
}

void Map::set_tile(int x, int y, int z, TileID tile)
{
    set_tile(index(x, y, z), tile);
}

void Map::set_material(int index, MaterialID material)
{
    if (index == -1)
        return;
    _materials[index] = material;
}

void Map::set_material(int x, int y, int z, MaterialID material)
{
    set_material(index(x, y, z), material);
}

void Map::compute_visibility(int x, int y, int z)
{
    int i;

    i = index(x, y, z);

    if (i == -1)
        return;
    if ((_tiles[i] == TileID::None)
        ||(x > 0 && _tiles[index(x - 1, y, z)] == TileID::None)
        || (x < _width - 1 && _tiles[index(x + 1, y, z)] == TileID::None)
        || (y > 0 && _tiles[index(x, y - 1, z)] == TileID::None)
        || (y < _height - 1 && _tiles[index(x, y + 1, z)] == TileID::None)
//        || (z > 0 && _tiles[index(x, y, z - 1)] == TileID::None)
//        || (z < _depth - 1 && _tiles[index(x, y, z + 1)] == TileID::None)
        )
        _visible[i] = true;
}

void Map::compute_visibility()
{
    for (int z = 0; z < _depth; ++z)
    {
        for (int y = 0; y < _height; ++y)
        {
            for (int x = 0; x < _width; ++x)
            {
                compute_visibility(x, y, z);
            }
        }
    }
}
