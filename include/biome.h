#ifndef BIOME_H
#define BIOME_H

#include <types.h>
#include <std/sparse_array.h>
#include <archive.h>
#include <color.h>

struct Biome
{
    char*           name;
    Color           color;
    Color           color_bg;
    uint16_t        symbol;
    u16             height_min;
    u16             height_max;
    u16             temperature_min;
    u16             temperature_max;
    u16             rain_min;
    u16             rain_max;
    u16             drainage_min;
    u16             drainage_max;
    MaterialID      layers[3];
    u8              noise_amplitude;
    float           noise_power;
    u16             tree_density;
    TileID          tree_tile;
    MaterialID      tree_material;
    bool            no_embark:1;

    static SparseArray<Biome> _data;

    static const Biome&     from_id(uint16_t id) { return _data[id]; }
    static size_t           count() { return _data.size(); }
    static void             load(Archive& archive);
};

#endif
