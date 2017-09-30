#include <window/window.h>
#include <window/sdl2.h>

Window::Window()
{

}

Window::~Window()
{

}

Window* Window::create(WindowType type, WindowRenderApi api, int major, int minor)
{
    switch (type)
    {
        case WindowType::SDL2:
            return WindowSDL2::create(api, major, minor);
    }
    return nullptr;
}
