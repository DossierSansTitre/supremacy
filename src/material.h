#ifndef MATERIAL_H
#define MATERIAL_H

#include <material_id.h>

struct Material
{
    int     color;
    int     bgcolor;
    int     dim_color;
    int     dim_bgcolor;

    static const Material materials[];

    static const Material& from_id(MaterialID id) { return materials[static_cast<int>(id)]; }
};

#endif
