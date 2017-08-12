#ifndef ENGINE_GAME_ENGINE_H
#define ENGINE_GAME_ENGINE_H

#include <non_copyable.h>

class GameScene;
class Window;
class Keyboard;

class GameEngine
{
public:
    GameEngine(Window& window, Keyboard& keyboard);
    ~GameEngine();

    void set_scene(GameScene* new_scene);
    void loop();
    void stop();

private:
    void update();
    void render();
    void handle_events();

    GameScene*  _scene;
    GameScene*  _next_scene;
    Window&     _window;
    Keyboard&   _keyboard;
    bool        _running;
};

#endif
