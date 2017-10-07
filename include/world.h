#ifndef WORLD_H
#define WORLD_H

#include <math/vector.h>
#include <map.h>
#include <items.h>
#include <actors.h>

class Game;

struct World
{
    World(u16 world_id, u32 region_id, Vector3i size)
    : world_id(world_id)
    , region_id(region_id)
    {
        map.create(size.x, size.y, size.z);
    }

    u16         world_id;
    u32         region_id;
    Vector3i    camera;
    Map         map;
    Items       items;
    Actors      actors;
};


#endif
