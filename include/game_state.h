#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <cstdint>
#include <non_copyable.h>
#include <thread_pool.h>
#include <map.h>
#include <fps_counter.h>
#include <actors.h>
#include <items.h>
#include <window.h>
#include <keyboard.h>
#include <renderer.h>
#include <ui_state.h>
#include <math/vector.h>
#include <draw_buffer.h>

struct GameState : private NonCopyable
{
    GameState();

    bool        running;
    ThreadPool  thread_pool;
    Window      window;
    Keyboard    keyboard;
    Renderer*   renderer;
    DrawBuffer  draw_buffer;
    Map         map;
    FpsCounter  fps_counter_update;
    FpsCounter  fps_counter_render;
    uint64_t    tick;
    uint64_t    tick_render;
    UiStateID   ui_state;
    Vector3i    camera;
    Vector3i    cursor;
    Vector3i    selection[2];
    bool        selected_first;
    uint32_t    seed;
    Actors      actors;
    Items       items;
    bool        vsync;
};

#endif
