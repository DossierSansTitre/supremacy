#include <engine/game.h>
#include <log.h>
#include <window.h>

Game::Game(Window& window)
: _window(window)
, _renderer(_thread_pool)
{

}

Game::~Game()
{
    delete _scene;
    delete _next_scene;
}

void Game::loop()
{
    using Clock = std::chrono::high_resolution_clock;
    using Timepoint = Clock::time_point;

    static const int update_hz = 20;
    static const double update_delay = 1.0 / update_hz;
    double update_acc = 0.0;
    double delta;

    Timepoint t0;
    Timepoint t1;

    _running = true;
    _scene = _next_scene;
    _next_scene = nullptr;
    _scene->setup();

    log_line(LogLevel::Info, "Engine started");

    t0 = Clock::now();
    while (_running)
    {
        render();

        if (!_window.focus())
            SDL_Delay(50);

        while (update_acc >= update_delay)
        {
            update_acc -= update_delay;
            handle_events();
            update();

            if (_next_scene)
            {
                _scene->teardown();
                delete _scene;
                _scene = _next_scene;
                _next_scene = nullptr;
                _scene->setup();
                render();
            }
        }

        t1 = Clock::now();
        delta = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count();

        update_acc += delta;
        t0 = t1;
    }
    _scene->teardown();
}


void Game::stop()
{
    _running = false;
    log_line(LogLevel::Info, "Engine stopped");
}

void Game::update()
{
    _scene->update();
}

void Game::render()
{
    _renderer.clear(_draw_buffer, _window);
    _scene->render(_draw_buffer);
    _renderer.render(_draw_buffer);
    _window.swap();
}

void Game::handle_events()
{
    SDL_Event event;

    _keyboard.tick();
    while (_window.poll_event(event))
    {
        switch (event.type)
        {
            case SDL_KEYDOWN:
                _keyboard.press_key(event.key.keysym.scancode);
                break;
            case SDL_KEYUP:
                _keyboard.release_key(event.key.keysym.scancode);
                break;
            default:
                break;
        }
    }
}
