#ifndef GAME_H
#define GAME_H

#include <cstdint>
#include <non_copyable.h>
#include <map.h>
#include <fps_counter.h>
#include <actors.h>
#include <items.h>
#include <window.h>
#include <keyboard.h>
#include <renderer.h>
#include <ui_state.h>

struct Game : private NonCopyable
{
    bool        running;
    Window      window;
    Keyboard    keyboard;
    Renderer    renderer;
    Map         map;
    FpsCounter  fps_counter_update;
    FpsCounter  fps_counter_render;
    UiState     ui;
    Vec3        camera;
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
