#ifndef WORLD_H
#define WORLD_H

#include <math/vector.h>
#include <map.h>
#include <items.h>
#include <actors.h>

class Game;

struct World
{
    World(Vector3i size)
    {
        map.create(size.x, size.y, size.z);
    }

    Vector3i    camera;
    Map         map;
    Items       items;
    Actors      actors;
};


#endif
