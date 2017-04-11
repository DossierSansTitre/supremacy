#ifndef GAME_H
#define GAME_H

#include <non_copyable.h>
#include <screen.h>
#include <map.h>

struct Game : private NonCopyable
{
    bool    running;
    Screen  screen;
    Map     map;
};

void game_loop(Game& game);
void game_event(Game& game);
void game_update(Game& game);
void game_render(Game& game);

#endif
