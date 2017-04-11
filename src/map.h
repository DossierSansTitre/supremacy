#ifndef MAP_H
#define MAP_H

#include <vector>
#include <tile_id.h>
#include <material_id.h>

class Map
{
public:
    Map();
    ~Map();

    void        create(int width, int height);
    void        destroy();

    int         width() { return _width; }
    int         height() { return _height; }

    void        at(int x, int y, TileID& out_tile, MaterialID& out_material);
    TileID      tile_at(int index);
    TileID      tile_at(int x, int y);
    MaterialID  material_at(int index);
    MaterialID  material_at(int x, int y);

    int         index(int x, int y);

    void        set_tile(int index, TileID tile);
    void        set_tile(int x, int y, TileID tile);
    void        set_material(int index, MaterialID material);
    void        set_material(int x, int y, MaterialID material);

private:
    int                     _width;
    int                     _height;
    TileID*                 _tiles;
    MaterialID*             _materials;
};

void generate_map(Map& map);

#endif
