#ifndef TILE_H
#define TILE_H

#include <cstdint>
#include <std/array.h>
#include <tile_id.h>
#include <archive.h>

struct Tile
{
    uint16_t    sym;
    uint16_t    dim_sym;
    uint16_t    dropping_frequency;
    bool        walkable;

    static Array<Tile> data;

    static const Tile& from_id(TileID id) { return data[static_cast<int>(id)]; }
    static void load(Archive& archive);
};

#endif
