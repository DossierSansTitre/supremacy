#ifndef WINDOW_WIN32_VULKAN_H
#define WINDOW_WIN32_VULKAN_H

#include <windows.h>
#include <window/win32.h>

class WindowWin32Vulkan : public WindowWin32
{
public:
	WindowWin32Vulkan(HWND window);
	~WindowWin32Vulkan();

	static WindowWin32Vulkan* create(HWND window, int major, int minor);

	void    swap() override;
	void    vsync(bool sync) override;

private:
};

#endif
