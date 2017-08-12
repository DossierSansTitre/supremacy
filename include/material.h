#ifndef MATERIAL_H
#define MATERIAL_H

#include <std/sparse_array.h>
#include <types.h>
#include <color.h>
#include <archive.h>

struct Material
{
    Color   color;
    Color   color_bg;
    ItemID  dropping_item;
    bool    minable;

    static SparseArray<Material> _data;

    static const Material& from_id(MaterialID id) { return _data[id]; }

    static void load(Archive& archive);
};

#endif
