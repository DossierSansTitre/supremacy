#ifndef RENDERER_VULKAN_H
#define RENDERER_VULKAN_H

#include <rendering/renderer.h>

class RendererVulkan : public Renderer
{
public:
	RendererVulkan(Window& window, DrawBuffer& draw_buffer);
	virtual ~RendererVulkan();

	virtual void    clear() override;
	virtual void    render() override;

	void    resize(Vector2u size);

private:
};

#endif
