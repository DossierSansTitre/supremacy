#include <rendering/renderer.h>

Renderer::Renderer(Window& window, DrawBuffer& draw_buffer)
: _window(window)
, _draw_buffer(draw_buffer)
{

}

Renderer::~Renderer()
{

}
