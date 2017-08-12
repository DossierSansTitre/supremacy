#ifndef TILE_H
#define TILE_H

#include <types.h>
#include <std/sparse_array.h>
#include <archive.h>

struct Tile
{
    uint16_t    sym;
    uint16_t    dim_sym;
    uint16_t    dropping_frequency;
    bool        walkable:1;
    bool        block:1;
    bool        support:1;
    bool        move_up:1;
    bool        move_down:1;

    static SparseArray<Tile> _data;

    static const Tile& from_id(TileID id) { return _data[id]; }
    static void load(Archive& archive);
};

#endif
