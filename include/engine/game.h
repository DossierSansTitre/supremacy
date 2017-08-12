#ifndef GAME_H
#define GAME_H

#include <non_copyable.h>
#include <engine/game_engine.h>
#include <keyboard.h>

class Window;
class GameScene;
class Game : NonCopyable
{
public:
    Game(Window& window);
    ~Game();

    Window&     window() { return _window; }
    Keyboard&   keyboard() { return _keyboard; }

    template <typename T, typename... Ts>
    T* set_scene(Ts... args)
    {
        T* scene;

        scene = new T(args...);
        scene->set_game(this);
        _engine.set_scene(scene);
        return scene;
    }

private:
    Window&     _window;
    Keyboard    _keyboard;
    GameEngine  _engine;
};

#endif
