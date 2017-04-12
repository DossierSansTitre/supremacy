#ifndef GAME_H
#define GAME_H

#include <non_copyable.h>
#include <screen.h>
#include <map.h>
#include <cstdint>

struct Game : private NonCopyable
{
    bool        running;
    Screen      screen;
    Map         map;
    int         camera_depth;
    uint32_t    seed;
};

void game_loop(Game& game);
void game_event(Game& game);
void game_update(Game& game);
void game_render(Game& game);

#endif
