#include <window/window.h>
#include <window/sdl2.h>

#if defined(__APPLE__)
# include <window/cocoa.h>
#endif

#if defined(WIN32)
# include <window/win32.h>
#endif

Window::Window()
: _scale(1.f)
{

}

Window::~Window()
{

}

void Window::vsync(bool sync)
{
    _vsync = sync;
}

void Window::toggle_vsync()
{
    vsync(!_vsync);
}

Window* Window::create(WindowType type, WindowRenderApi api, int major, int minor)
{
    switch (type)
    {
        case WindowType::SDL2:
            return WindowSDL2::create(api, major, minor);
        case WindowType::Cocoa:
#if defined(__APPLE__)
            return WindowCocoa::create(api, major, minor);
#else
            return nullptr;
#endif
		case WindowType::Win32:
#if defined(WIN32)
			return WindowWin32::create(api, major, minor);
#else
			return nullptr;
#endif
        default:
            return nullptr;
    }
}
