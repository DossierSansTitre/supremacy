#ifndef MAP_H
#define MAP_H

#include <vector>
#include <cstdint>
#include <non_copyable.h>
#include <tile_id.h>
#include <material_id.h>

class Map : private NonCopyable
{
public:
    Map();
    ~Map();

    int width() const
    {
        return _width;
    }

    int height() const
    {
        return _height;
    }

    int depth() const
    {
        return _depth;
    }

    void at(int x, int y, int z, TileID& out_tile, MaterialID& out_material) const
    {
        int i = index(x, y, z);
        out_tile = tile_at(i);
        out_material = material_at(i);
    }

    TileID tile_at(int i) const
    {
        if (i == -1)
            return TileID::None;
        return _tiles[i];
    }

    TileID tile_at(int x, int y, int z) const
    {
        return tile_at(index(x, y, z));
    }

    MaterialID material_at(int i) const
    {
        if (i == -1)
            return MaterialID::None;
        return _materials[i];
    }

    MaterialID material_at(int x, int y, int z) const
    {
        return material_at(index(x, y, z));
    }

    bool visible(int i) const
    {
        if (i == -1)
            return false;
        return _visible[i];
    }

    bool visible(int x, int y, int z) const
    {
        return visible(index(x, y, z));
    }

    int index(int x, int y, int z) const
    {
        if (x < 0 || x >= _width || y < 0 || y >= _height || z < 0 || z >= _depth)
            return -1;
        return x + y * _width + z * _width * _height;
    }

    void        create(int width, int height, int depth);
    void        destroy();

    void        set_tile(int index, TileID tile);
    void        set_tile(int x, int y, int z, TileID tile);
    void        set_material(int index, MaterialID material);
    void        set_material(int x, int y, int z, MaterialID material);

    void        compute_visibility(int x, int y, int z);
    void        compute_visibility();

private:
    int                     _width;
    int                     _height;
    int                     _depth;
    TileID*                 _tiles;
    MaterialID*             _materials;
    std::vector<bool>       _visible;
};

void generate_map(Map& map, uint32_t seed);

#endif
