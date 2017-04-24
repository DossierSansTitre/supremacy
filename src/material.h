#ifndef MATERIAL_H
#define MATERIAL_H

#include <material_id.h>
#include <color.h>

struct Material
{
    Color   color;
    Color   color_bg;
    bool    minable;

    static const Material materials[];

    static const Material& from_id(MaterialID id) { return materials[static_cast<int>(id)]; }
};

#endif
