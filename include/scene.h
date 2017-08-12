#ifndef SCENE_H
#define SCENE_H

#include <non_copyable.h>
#include <game.h>

class Game;
class Scene : NonCopyable
{
public:
    Scene(Game& game);
    virtual ~Scene();

    const Game& game() const { return _game; }
    Game& game() { return _game; }

    virtual void setup();
    virtual void teardown();
    virtual void update();
    virtual void render();

private:
    Game&   _game;
};

#endif
