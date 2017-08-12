#ifndef GAME_H
#define GAME_H

#include <non_copyable.h>
#include <engine/game_scene.h>
#include <keyboard.h>
#include <thread_pool.h>
#include <renderer.h>
#include <draw_buffer.h>

class Window;
class GameScene;
class Game : NonCopyable
{
public:
    Game(Window& window);
    ~Game();

    Window&     window() { return _window; }
    Keyboard&   keyboard() { return _keyboard; }
    ThreadPool& thread_pool() { return _thread_pool; }
    Renderer&   renderer() { return _renderer; }
    DrawBuffer& draw_buffer() { return _draw_buffer; }

    template <typename T, typename... Ts>
    T* set_scene(Ts... args)
    {
        T* scene;

        scene = new T(args...);
        scene->set_game(*this);
        if (_next_scene)
            delete _next_scene;
        _next_scene = scene;
        return scene;
    }

    void loop();
    void stop();

private:
    void    update();
    void    render();
    void    handle_events();

    Window&     _window;
    Keyboard    _keyboard;
    ThreadPool  _thread_pool;
    Renderer    _renderer;
    DrawBuffer  _draw_buffer;
    GameScene*  _scene;
    GameScene*  _next_scene;
    bool        _running;
};

#endif
