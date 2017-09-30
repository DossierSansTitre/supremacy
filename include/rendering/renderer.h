#ifndef RENDERER_H
#define RENDERER_H

#include <non_copyable.h>
#include <draw_buffer.h>
#include <window/window.h>

class Renderer : private NonCopyable
{
public:
    Renderer(Window& window, DrawBuffer& draw_buffer);
    virtual ~Renderer();

    virtual void clear() = 0;
    virtual void render() = 0;

protected:
    Window&             _window;
    DrawBuffer&         _draw_buffer;
};

#endif
