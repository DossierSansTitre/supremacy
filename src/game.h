#ifndef GAME_H
#define GAME_H

#include <non_copyable.h>
#include <world.h>
#include <screen.h>

struct Game : private NonCopyable
{
    int     debug;
    bool    running;
    Screen  screen;
};

void game_loop(Game& game);
void game_event(Game& game);
void game_update(Game& game);
void game_render(Game& game);

#endif
