#include <window/win32_vulkan.h>

WindowWin32Vulkan::WindowWin32Vulkan(HWND window)
: WindowWin32(window)
{

}

WindowWin32Vulkan::~WindowWin32Vulkan()
{

}

WindowWin32Vulkan* WindowWin32Vulkan::create(HWND window, int major, int minor)
{
	return new WindowWin32Vulkan(window);
}

void WindowWin32Vulkan::swap()
{

}

void WindowWin32Vulkan::vsync(bool sync)
{
	Window::vsync(sync);
}