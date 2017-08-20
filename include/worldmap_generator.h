#ifndef WORLDMAP_GENERATOR_H
#define WORLDMAP_GENERATOR_H

#include <worldmap.h>

class WorldmapGenerator
{
public:
    WorldmapGenerator();
    ~WorldmapGenerator();

    Worldmap* generate(u16 id, Vector2i size);
};

#endif
