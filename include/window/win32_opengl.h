#ifndef WINDOW_WIN32_OPENGL_H
#define WINDOW_WIN32_OPENGL_H

#include <windows.h>
#include <window/win32.h>

class WindowWin32OpenGL : public WindowWin32
{
public:
	WindowWin32OpenGL(HWND window, HDC dc, HGLRC gl);
	~WindowWin32OpenGL();

	static void init();
	static WindowWin32OpenGL* create(HWND window, int major, int minor);

	void    swap() override;
	void    vsync(bool sync) override;

private:
	HDC		_dc;
	HGLRC	_gl;
};

#endif
