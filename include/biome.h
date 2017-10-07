#ifndef BIOME_H
#define BIOME_H

#include <types.h>
#include <std/sparse_array.h>
#include <archive.h>
#include <color.h>

struct Biome
{
    enum class LayerType
    {
        None,
        Fill,
        Octave,
        Smooth,
        Replace
    };

    struct LayerFill
    {
        uint16_t height;
        uint16_t tile;
        uint16_t material;
    };

    struct LayerOctave
    {
        float       persistance;
        uint16_t    octaves;
        float       power;
        uint16_t    a;
        uint16_t    b;
        uint16_t    tile;
        uint16_t    material;
    };

    struct LayerSmooth
    {
        uint16_t    from;
        uint16_t    to;
    };

    struct LayerReplace
    {
        uint16_t    from_tile;
        uint16_t    from_material;
        uint16_t    to_tile;
        uint16_t    to_material;
        uint32_t    frequency;
    };

    struct Layer
    {
        LayerType   type;
        union
        {
            LayerFill       fill;
            LayerOctave     octave;
            LayerSmooth     smooth;
            LayerReplace    replace;
        };
    };

    char*           name;
    Array<Layer>    layers;
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
