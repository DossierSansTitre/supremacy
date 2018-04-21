#include <GL/glew.h>
#include <GL/wglew.h>
#include <window/win32_opengl.h>

static ATOM dummy_window_class = 0;

static PIXELFORMATDESCRIPTOR dummy_pfd = {
	sizeof(PIXELFORMATDESCRIPTOR),
	1,
	PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
	PFD_TYPE_RGBA,
	32,
	0, 0, 0, 0, 0, 0,
	0,
	0,
	0,
	0, 0, 0, 0,
	24,
	8,
	0,
	PFD_MAIN_PLANE,
	0,
	0, 0, 0
};

static LRESULT dummy_winproc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	switch (msg)
	{
	case WM_CREATE:
		return 0;
	default:
		return DefWindowProcW(hwnd, msg, w, l);
	}
}

static void register_dummy_window_class()
{
	if (dummy_window_class)
		return;

	WNDCLASSEXW klass;
	klass.cbSize = sizeof(WNDCLASSEXW);
	klass.style = CS_OWNDC;
	klass.lpfnWndProc = &dummy_winproc;
	klass.cbClsExtra = 0;
	klass.cbWndExtra = 0;
	klass.hInstance = NULL;
	klass.hIcon = NULL;
	klass.hCursor = LoadCursorW(NULL, IDC_ARROW);
	klass.hbrBackground = NULL;
	klass.lpszMenuName = NULL;
	klass.lpszClassName = L"SupremacyWindowDummy";
	klass.hIconSm = NULL;
	dummy_window_class = RegisterClassExW(&klass);
}

static HWND create_dummy_window()
{
	HWND hwnd;

	register_dummy_window_class();
	hwnd = CreateWindowExW(
		0x0,
		(LPCTSTR)dummy_window_class,
		L"",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800,
		600,
		NULL,
		NULL,
		NULL,
		NULL
	);
	return hwnd;
}

static HGLRC create_opengl(HWND win, HDC dc, int major, int minor)
{
	int gl_attr[32] = { 0 };
	int count = 0;

	static const int attr[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0,
	};

	gl_attr[count++] = WGL_CONTEXT_MAJOR_VERSION_ARB;
	gl_attr[count++] = major;
	gl_attr[count++] = WGL_CONTEXT_MINOR_VERSION_ARB;
	gl_attr[count++] = minor;

	gl_attr[count++] = WGL_CONTEXT_PROFILE_MASK_ARB;
	if (major >= 3)
	{
		gl_attr[count++] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
	}
	else
	{
		gl_attr[count++] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
	}

	int pf;
	UINT pf_count;
	HGLRC gl;

	wglChoosePixelFormatARB(dc, attr, NULL, 1, &pf, &pf_count);
	SetPixelFormat(dc, pf, &dummy_pfd);
	gl = wglCreateContextAttribsARB(dc, NULL, gl_attr);
	wglMakeCurrent(dc, gl);
	wglSwapIntervalEXT(1);
	return gl;
}

WindowWin32OpenGL::WindowWin32OpenGL(HWND window, HDC dc, HGLRC gl)
: WindowWin32(window)
, _dc(dc)
, _gl(gl)
{

}

WindowWin32OpenGL::~WindowWin32OpenGL()
{
	wglMakeCurrent(_dc, NULL);
	ReleaseDC(_window, _dc);
	wglDeleteContext(_gl);
}

void WindowWin32OpenGL::init()
{
	static bool loaded = false;

	if (loaded)
		return;

	HWND dummy_win = create_dummy_window();
	HDC dummy_dc = GetDC(dummy_win);
	int pf = ChoosePixelFormat(dummy_dc, &dummy_pfd);
	SetPixelFormat(dummy_dc, pf, &dummy_pfd);
	HGLRC dummy_gl = wglCreateContext(dummy_dc);
	wglMakeCurrent(dummy_dc, dummy_gl);
	glewInit();
	wglMakeCurrent(dummy_dc, NULL);
	ReleaseDC(dummy_win, dummy_dc);
	wglDeleteContext(dummy_gl);
	DestroyWindow(dummy_win);
	loaded = true;
}

WindowWin32OpenGL* WindowWin32OpenGL::create(HWND window, int major, int minor)
{
	HDC dc;
	HGLRC gl;

	dc = GetDC(window);
	gl = create_opengl(window, dc, major, minor);
	if (!gl)
	{
		ReleaseDC(window, dc);
		DestroyWindow(window);
		return nullptr;
	}
	return new WindowWin32OpenGL(window, dc, gl);
}

void WindowWin32OpenGL::swap()
{
	SwapBuffers(_dc);
}

void WindowWin32OpenGL::vsync(bool sync)
{
	wglSwapIntervalEXT(sync ? 1 : 0);
	Window::vsync(sync);
}