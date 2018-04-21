#include <rendering/renderer_vulkan.h>
#include <log.h>

RendererVulkan::RendererVulkan(Window& window, DrawBuffer& draw_buffer)
: Renderer(window, draw_buffer)
{
	log_line(LogLevel::Info, "Using the Vulkan Renderer");
}

RendererVulkan::~RendererVulkan()
{

}

void RendererVulkan::clear()
{
	_draw_buffer.resize(_window.width() / 32, _window.height() / 32);
}

void RendererVulkan::render()
{

}


void RendererVulkan::resize(Vector2u size)
{
	auto& db = _draw_buffer;

	db.resize(size.x, size.y);
}