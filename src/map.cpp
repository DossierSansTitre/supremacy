#include <map.h>

Map::Map()
: _width(0)
, _height(0)
, _tiles(nullptr)
, _materials(nullptr)
{

}

Map::~Map()
{
    destroy();
}

void Map::create(int width, int height)
{
    size_t size;

    _width = width;
    _height = height;

    size = width * height;

    _tiles = new TileID[size];
    _materials = new MaterialID[size];

    std::fill(_tiles, _tiles + size, TileID::None);
    std::fill(_materials, _materials + size, MaterialID::None);
}

void Map::destroy()
{
    delete [] _tiles;
    delete [] _materials;
    _width = 0;
    _height = 0;
    _tiles = nullptr;
    _materials = nullptr;
}

void Map::at(int x, int y, TileID &tile_id, MaterialID &material_id)
{
    int i = index(x, y);

    tile_id = tile_at(i);
    material_id = material_at(i);
}

TileID Map::tile_at(int i)
{
    if (i == -1)
        return TileID::None;
    return _tiles[i];
}

TileID Map::tile_at(int x, int y)
{
    return tile_at(index(x, y));
}

MaterialID Map::material_at(int i)
{
    if (i == -1)
        return MaterialID::None;
    return _materials[i];
}

MaterialID Map::material_at(int x, int y)
{
    return material_at(index(x, y));
}

int Map::index(int x, int y)
{
    if (x < 0 || x >= _width || y < 0 || y >= _height)
        return -1;
    return x + y * _width;
}


void Map::set_tile(int index, TileID tile)
{
    if (index == -1)
        return;
    _tiles[index] = tile;
}

void Map::set_tile(int x, int y, TileID tile)
{
    set_tile(index(x, y), tile);
}

void Map::set_material(int index, MaterialID material)
{
    if (index == -1)
        return;
    _materials[index] = material;
}

void Map::set_material(int x, int y, MaterialID material)
{
    set_material(index(x, y), material);
}
