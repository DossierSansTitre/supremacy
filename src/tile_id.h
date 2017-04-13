#ifndef TILE_ID_H
#define TILE_ID_H

#include <cstdint>

enum class TileID : uint8_t {
    None,
    Block,
    Stairs,
    Ramp,
    Floor
};

#endif
