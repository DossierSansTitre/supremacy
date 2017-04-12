#ifndef MAP_H
#define MAP_H

#include <cstdint>
#include <non_copyable.h>
#include <tile_id.h>
#include <material_id.h>

class Map : private NonCopyable
{
public:
    Map();
    ~Map();

    void        create(int width, int height, int depth);
    void        destroy();

    int         width() const { return _width; }
    int         height() const { return _height; }
    int         depth() const { return _depth; }

    void        at(int x, int y, int z, TileID& out_tile, MaterialID& out_material);
    TileID      tile_at(int index);
    TileID      tile_at(int x, int y, int z);
    MaterialID  material_at(int index);
    MaterialID  material_at(int x, int y, int z);

    int         index(int x, int y, int z);

    void        set_tile(int index, TileID tile);
    void        set_tile(int x, int y, int z, TileID tile);
    void        set_material(int index, MaterialID material);
    void        set_material(int x, int y, int z, MaterialID material);

private:
    int                     _width;
    int                     _height;
    int                     _depth;
    TileID*                 _tiles;
    MaterialID*             _materials;
};

void generate_map(Map& map, uint32_t seed);

#endif
