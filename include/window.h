#ifndef WINDOW_H
#define WINDOW_H

#include <opengl.h>
#include <non_copyable.h>

class Window : private NonCopyable
{
public:
    Window();
    ~Window();

    void    init();
    void    destroy();

    uint32_t    width() const { return _width; }
    uint32_t    height() const { return _height; }
    bool        focus() const { return _focus; }

    void    swap();
    bool    poll_event(SDL_Event& event);

private:
    SDL_Window*     _window;
    SDL_GLContext   _opengl;
    uint32_t        _width;
    uint32_t        _height;
    bool            _focus;
};

#endif
