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

    int         width() { return _width; }
    int         height() { return _height; }
    void        generate();
    void        at(int, int, TileID&, MaterialID&);
    TileID      tile_at(int, int);
    MaterialID  material_at(int, int);
    int         index(int, int);

private:
    int                     _width;
    int                     _height;
    std::vector<TileID>     _tiles;
    std::vector<MaterialID> _materials;
};

#endif
