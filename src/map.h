#ifndef MAP_H
#define MAP_H

#include <vector>
#include <tile_id.h>
#include <material_id.h>

class Map
{
public:
    Map(int width, int height);
    ~Map() {}

    void        create(int width, int height);
    int         width() { return _width; }
    int         height() { return _height; }
    void        at(int, int, TileID&, MaterialID&);
    TileID      tile_at(int index);
    TileID      tile_at(int x, int y);
    MaterialID  material_at(int index);
    MaterialID  material_at(int x, int y);
    int         index(int x, int y);

private:
    int                     _width;
    int                     _height;
    int                     _depth;
    std::vector<TileID>     _tiles;
    std::vector<MaterialID> _materials;
};

#endif
