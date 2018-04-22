#ifndef WINDOW_WIN32_VULKAN_H
#define WINDOW_WIN32_VULKAN_H

#include <windows.h>
#include <vulkan/vulkan.h>
#include <window/win32.h>

class WindowWin32Vulkan : public WindowWin32
{
public:
	WindowWin32Vulkan(HWND window, VkInstance instance);
	~WindowWin32Vulkan();

	static WindowWin32Vulkan* create(HWND window, int major, int minor);

	void    swap() override;
	void    vsync(bool sync) override;

private:
	void	init_device();
	void	init_surface();
	void	init_swapchain();

	VkInstance			_instance;
	VkPhysicalDevice	_physical_device;
	VkDevice			_device;
	VkSurfaceKHR		_surface;
	VkPresentModeKHR	_present_mode;
	VkPresentModeKHR	_present_mode_vsync;
	VkSwapchainKHR		_swapchain;
};

#endif
