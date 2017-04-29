#ifndef GAME_H
#define GAME_H

#include <cstdint>
#include <non_copyable.h>
#include <map.h>
#include <fps_counter.h>
#include <actors.h>
#include <items.h>
#include <window.h>
#include <renderer.h>

struct Game : private NonCopyable
{
    bool        running;
    Window      window;
    Renderer    renderer;
    Map         map;
    FpsCounter  fps_counter_update;
    FpsCounter  fps_counter_render;
    int         camera_x;
    int         camera_y;
    int         camera_depth;
    uint32_t    seed;
    Actors      actors;
    Items       items;
    bool        vsync;
};

void game_loop(Game& game);
void game_event(Game& game);
void game_update(Game& game);
void game_draw(Game& game);

void game_ai(Game& game);

#endif
