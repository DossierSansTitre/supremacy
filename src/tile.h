#ifndef TILE_H
#define TILE_H

#include <tile_id.h>

struct Tile
{
    int     sym;
    int     dim_sym;

    static const Tile tiles[];

    static const Tile& from_id(TileID id) { return tiles[static_cast<int>(id)]; }
};

#endif
