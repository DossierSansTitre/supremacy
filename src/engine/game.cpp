#include <fstream>
#include <engine/game.h>
#include <log.h>
#include <window/window.h>
#include <opengl.h>
#include <rendering/renderer_opengl_legacy.h>
#include <rendering/renderer_opengl_shader.h>
#include <cli_options.h>

static void init_rng(Rng& rng)
{
    u64 s[2];
    std::ifstream stream("/dev/random", std::ios::binary);

    do
    {
        stream.read((char*)s, 16);
    } while (s[0] == 0 && s[1] == 0);
    rng.seed(s);
}

Game::Game()
: _window(nullptr)
, _renderer(nullptr)
, _scene(nullptr)
, _next_scene(nullptr)
{
    init_rng(_rng);
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

void Game::select_renderer()
{
    auto& opts = CliOptions::instance();
    WindowType window_type;

    if (opts.sdl)
        window_type = WindowType::SDL2;
    else
        window_type = WindowType::Cocoa;

    _window = nullptr;
    if (!opts.legacy)
        _window = Window::create(window_type, WindowRenderApi::OpenGL, 3, 2);
    if (_window)
    {
        log_line(LogLevel::Info, "OpenGL Info:");
        log_line(LogLevel::Info, "  %s", glGetString(GL_VERSION));
        log_line(LogLevel::Info, "  From: %s", glGetString(GL_VENDOR));
        log_line(LogLevel::Info, "  Renderer: %s", glGetString(GL_RENDERER));
        log_line(LogLevel::Info, "  GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
        _renderer = new RendererOpenGLShader(*_window, _draw_buffer);
    }
    else
    {
        _window = Window::create(window_type, WindowRenderApi::OpenGL, 2, 1);
        log_line(LogLevel::Info, "OpenGL Info:");
        log_line(LogLevel::Info, "  %s", glGetString(GL_VERSION));
        log_line(LogLevel::Info, "  From: %s", glGetString(GL_VENDOR));
        log_line(LogLevel::Info, "  Renderer: %s", glGetString(GL_RENDERER));
        log_line(LogLevel::Info, "  GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
        _renderer = new RendererOpenGLLegacy(*_window, _draw_buffer);
    }
}

void Game::update()
{
    _scene->update();
    _renderer->clear();
    _scene->render(_draw_buffer);
    _fps_counter_update.update();
}

void Game::render()
{
    _renderer->render();
    _window->swap();
    _fps_counter_render.update();
}

void Game::handle_events()
{
    _input.tick();
    _window->poll(_input);
}
