#ifndef WINDOW_WIN32_H
#define WINDOW_WIN32_H

#include <windows.h>
#include <window/window.h>

class WindowWin32 : public Window
{
public:
	WindowWin32(HWND window, HDC dc, HGLRC gl);
	~WindowWin32();

	static WindowWin32* create(WindowRenderApi api, int major, int minor);

	void    swap() override;
	void    poll(Input& input) override;
	void    vsync(bool sync) override;

private:
	HWND	_window;
	HDC		_dc;
	HGLRC	_gl;
};

#endif
