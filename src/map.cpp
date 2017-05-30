#include <algorithm>
#include <map.h>
#include <tile.h>

Map::Map()
: _width(0)
, _height(0)
, _depth(0)
, _tiles(nullptr)
, _materials(nullptr)
, _map_actions(nullptr)
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
    _map_actions = new MapAction[size];
    _visible.resize(size);
    _occupied.resize(size);
    _flash.resize(size);

    std::fill(_tiles, _tiles + size, TileID::None);
    std::fill(_materials, _materials + size, MaterialID::None);
    std::fill(_map_actions, _map_actions + size, MapAction::None);
    std::fill(_flash.data(), _flash.data() + size, Flash::None);
}

void Map::destroy()
{
    delete [] _tiles;
    delete [] _materials;
    delete [] _map_actions;
    _width = 0;
    _height = 0;
    _depth = 0;
    _tiles = nullptr;
    _materials = nullptr;
    _map_actions = nullptr;
    _visible.resize(0);
    _occupied.resize(0);
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
    post_update(Vec3(x, y, z));
}

void Map::set_tile(Vec3 pos, TileID tile)
{
    set_tile(pos.x, pos.y, pos.z, tile);
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

void Map::set_action(int x, int y, int z, MapAction action)
{
    int i;

    i = index(x, y, z);

    if (i == -1)
        return;
    _map_actions[i] = action;
    _map_actions_array.push_back({x, y, z});
}

void Map::set_occupied(Vec3 pos, bool occupied)
{
    int i = index(pos);

    if (i == -1)
        return;

    _occupied[i] = occupied;
}

void Map::set_flash(Vec3 pos, Flash flash)
{
    int i = index(pos);

    if (i == -1)
        return;
    if (static_cast<int>(_flash[i]) < static_cast<int>(flash))
    {
        _flash[i] = flash;
        _flash_reset.push_back(i);
    }
}

static bool transparent(TileID tile)
{
    if (tile == TileID::None)
        return true;
    if (Tile::from_id(tile).walkable)
        return true;
    return false;
}

void Map::compute_visibility(int x, int y, int z)
{
    int i;

    i = index(x, y, z);

    if (i == -1)
        return;
    if (transparent(_tiles[i])
        || (x > 0 && transparent(_tiles[index(x - 1, y, z)]))
        || (x < _width - 1 && transparent(_tiles[index(x + 1, y, z)]))
        || (y > 0 && transparent(_tiles[index(x, y - 1, z)]))
        || (y < _height - 1 && transparent(_tiles[index(x, y + 1, z)]))
//        || (z > 0 && _tiles[index(x, y, z - 1)] == TileID::None)
//        || (z < _depth - 1 && _tiles[index(x, y, z + 1)] == TileID::None)
        )
        _visible[i] = true;
}

void Map::compute_visibility()
{
    auto size = _visible.size();
    _visible.resize(0);
    _visible.resize(size);

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

void Map::tick()
{
    for (int i : _flash_reset)
        _flash[i] = Flash::None;
    _flash_reset.clear();
}

void Map::post_update(Vec3 pos)
{
    neighbor_updated(pos);
    neighbor_updated(pos + Vec3(1, 0, 0));
    neighbor_updated(pos + Vec3(-1, 0, 0));
    neighbor_updated(pos + Vec3(0, 1, 0));
    neighbor_updated(pos + Vec3(0, -1, 0));
    neighbor_updated(pos + Vec3(0, 0, 1));
    neighbor_updated(pos + Vec3(0, 0, -1));
}

void Map::neighbor_updated(Vec3 pos)
{
    bool collapse;

    static const Vec3 cardinal[] = {
        {1, 0, 0},
        {-1, 0, 0},
        {0, 1, 0},
        {0, -1, 0}
    };

    compute_visibility(pos.x, pos.y, pos.z);
    if (tile_at(pos) == TileID::Ramp)
    {
        collapse = true;
        for (int i = 0; i < 4; ++i)
        {
            if (tile_at(pos + cardinal[i]) == TileID::Block)
            {
                collapse = false;
                break;
            }
        }
        if (collapse)
            set_tile(pos, TileID::Floor);
    }
}
