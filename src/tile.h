#ifndef TILE_H
#define TILE_H

#include <cstdint>
#include <tile_id.h>

struct Tile
{
    uint16_t    sym;
    uint16_t    dim_sym;
    bool        walkable;

    static const Tile tiles[];

    static const Tile& from_id(TileID id) { return tiles[static_cast<int>(id)]; }
};

#endif
