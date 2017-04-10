#ifndef GAME_H
#define GAME_H

#include <non_copyable.h>
#include <world.h>

struct Game : private NonCopyable
{
public:
    World   world;
};

#endif
