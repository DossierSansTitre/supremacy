#ifndef WINDOW_WIN32_H
#define WINDOW_WIN32_H

#include <windows.h>
#include <window/window.h>

class WindowWin32 : public Window
{
public:
	WindowWin32(HWND window);
	~WindowWin32();

	static WindowWin32* create(WindowRenderApi api, int major, int minor);

	void    poll(Input& input) override;

protected:
	HWND	_window;
};

#endif
