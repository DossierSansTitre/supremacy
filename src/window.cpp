#include <window.h>
#include <iostream>

Window::Window()
: _window(nullptr)
, _focus(true)
{

}

Window::~Window()
{
    destroy();
}

void Window::init()
{
    int w;
    int h;
    SDL_DisplayMode dm;
    uint32_t sdl_flags;

    SDL_GetDesktopDisplayMode(0, &dm);
    sdl_flags = SDL_WINDOW_OPENGL;
    sdl_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    _window = SDL_CreateWindow(
        "Supremacy",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        dm.w,
        dm.h,
        sdl_flags
    );

    _opengl = SDL_GL_CreateContext(_window);
#if defined(_WIN32)
    glewInit();
#endif
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, dm.w, dm.h, 0, 1, -1);
    glMatrixMode(GL_MODELVIEW);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    int vsync;
    vsync = SDL_GL_SetSwapInterval(-1);
    if (vsync == -1)
    {
        vsync = SDL_GL_SetSwapInterval(1);
        if (vsync == -1)
            std::cout << "Could not enable vsync" << std::endl;
    }
    SDL_GetWindowSize(_window, &w, &h);
    _width = w;
    _height = h;
}

void Window::destroy()
{
    if (_window)
    {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
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
