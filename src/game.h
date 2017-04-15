#ifndef GAME_H
#define GAME_H

#include <cstdint>
#include <non_copyable.h>
#include <screen.h>
#include <map.h>
#include <fps_counter.h>
#include <actors.h>

struct Game : private NonCopyable
{
    bool        running;
    Screen      screen;
    Map         map;
    FpsCounter  fps_counter_update;
    FpsCounter  fps_counter_render;
    int         camera_x;
    int         camera_y;
    int         camera_depth;
    uint32_t    seed;
    Actors      actors;
};

void game_loop(Game& game);
void game_event(Game& game);
void game_update(Game& game);
void game_render(Game& game);

#endif
