#include <iostream>
#include <opengl.h>
#include <window.h>
#include <cli_options.h>

Window::Window(SDL_Window* window, SDL_GLContext opengl)
: _window(window)
, _opengl(opengl)
, _focus(true)
{
    int w;
    int h;

    SDL_GetWindowSize(_window, &w, &h);
    _width = w;
    _height = h;
}

Window::~Window()
{
    SDL_GL_MakeCurrent(_window, nullptr);
    SDL_GL_DeleteContext(_opengl);
    SDL_DestroyWindow(_window);
}

static SDL_Window* create_window()
{
    SDL_Window* window;
    SDL_DisplayMode dm;
    uint32_t sdl_flags;
    const auto& opts = CliOptions::instance();

    SDL_GetDesktopDisplayMode(0, &dm);
    sdl_flags = SDL_WINDOW_OPENGL;
    if (!opts.windowed)
        sdl_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    window = SDL_CreateWindow(
        "Supremacy",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        dm.w,
        dm.h,
        sdl_flags
    );
    return window;
}

static SDL_GLContext create_gl_context(SDL_Window* window)
{
    SDL_GLContext opengl;
    int vsync;

    opengl = SDL_GL_CreateContext(window);
    if (opengl == nullptr)
        return nullptr;
    glewInit();
    vsync = SDL_GL_SetSwapInterval(-1);
    if (vsync == -1)
        SDL_GL_SetSwapInterval(1);
    return opengl;
}

Window* Window::create(int opengl_major, int opengl_minor)
{
    SDL_GLprofile gl_profile;
    SDL_Window* window;
    SDL_GLContext opengl;

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, opengl_major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, opengl_minor);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);

    if (opengl_major >= 3)
        gl_profile = SDL_GL_CONTEXT_PROFILE_CORE;
    else
        gl_profile = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, gl_profile);
    window = create_window();
    if (window == nullptr)
        return nullptr;
    opengl = create_gl_context(window);
    if (opengl == nullptr)
    {
        SDL_DestroyWindow(window);
        return nullptr;
    }
    return new Window(window, opengl);
}

void Window::swap()
{
    SDL_GL_SwapWindow(_window);
}

bool Window::poll_event(SDL_Event& event)
{
    int ret;

    for (;;)
    {
        ret = SDL_PollEvent(&event);
        if (!ret)
            return false;
        if (event.type == SDL_WINDOWEVENT) {
            switch (event.window.event)
            {
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    _focus = true;
                    break;
                case SDL_WINDOWEVENT_FOCUS_LOST:
                    _focus = false;
                    break;
            }
        }
        else
        {
            break;
        }
    }
    return true;
}
