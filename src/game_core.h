#ifndef GAME_CORE_H
#define GAME_CORE_H

#include <non_copyable.h>
#include <game.h>

class GameCore : private NonCopyable
{
public:
    GameCore();
    ~GameCore();

    void loop();

private:
    Game    _game;
};

#endif
