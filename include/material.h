#ifndef MATERIAL_H
#define MATERIAL_H

#include <std/array.h>
#include <types.h>
#include <color.h>
#include <archive.h>

struct Material
{
    Color   color;
    Color   color_bg;
    ItemID  dropping_item;
    bool    minable;

    static Array<Material> data;

    static const Material& from_id(MaterialID id) { return data[static_cast<int>(id)]; }

    static void load(Archive& archive);
};

#endif
