#ifndef ENGINE_GAME_SCENE_H
#define ENGINE_GAME_SCENE_H

#include <non_copyable.h>
#include <draw_buffer.h>

class Game;
class GameScene : NonCopyable
{
public:
    GameScene();
    virtual ~GameScene();

    Game& game() { return *_game; }
    void set_game(Game& game) { _game = &game; }

    virtual void setup();
    virtual void teardown();
    virtual void update();
    virtual void render(DrawBuffer& draw_buffer);

private:
    Game*   _game;
};

#endif
