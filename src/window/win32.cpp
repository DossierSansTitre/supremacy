#include <GL/glew.h>
#include <GL/wglew.h>
#include <window/win32.h>
#include <cli_options.h>
#include <keyboard.h>

static ATOM window_class = 0;
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

static LRESULT winproc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	switch (msg)
	{
	case WM_CREATE:
		return 0;
	case WM_KEYDOWN:
		return 0;
	case WM_KEYUP:
		return 0;
	case WM_CHAR:
		return 0;
	default:
		return DefWindowProcW(hwnd, msg, w, l);
	}
}

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

static void register_window_class()
{
	if (window_class)
		return;

	WNDCLASSEXW klass;
	klass.cbSize = sizeof(WNDCLASSEXW);
	klass.style = CS_OWNDC;
	klass.lpfnWndProc = &winproc;
	klass.cbClsExtra = 0;
	klass.cbWndExtra = 0;
	klass.hInstance = NULL;
	klass.hIcon = NULL;
	klass.hCursor = LoadCursorW(NULL, IDC_ARROW);
	klass.hbrBackground = NULL;
	klass.lpszMenuName = NULL;
	klass.lpszClassName = L"SupremacyWindow";
	klass.hIconSm = NULL;
	window_class = RegisterClassExW(&klass);
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

static void init_opengl()
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

static HWND create_window()
{
	HWND hwnd;
	HMONITOR mon;
	MONITORINFO mon_info;

	register_window_class();
	hwnd = CreateWindowExW(
		0x0,
		(LPCTSTR)window_class,
		L"Supremacy",
		WS_POPUP | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800,
		600,
		NULL,
		NULL,
		NULL,
		NULL
	);
	mon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY);
	mon_info.cbSize = sizeof(MONITORINFO);
	GetMonitorInfoW(mon, &mon_info);
	SetWindowPos(
		hwnd,
		HWND_TOPMOST,
		mon_info.rcMonitor.left,
		mon_info.rcMonitor.top,
		mon_info.rcMonitor.right - mon_info.rcMonitor.left,
		mon_info.rcMonitor.bottom - mon_info.rcMonitor.top,
		0x0);
	return hwnd;
}

static HGLRC create_opengl(HWND win, HDC dc)
{
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

	static const int gl_attr[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 2,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

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

WindowWin32::WindowWin32(HWND window, HDC dc, HGLRC gl)
: _window(window)
, _dc(dc)
, _gl(gl)
{
	RECT rect;

	GetClientRect(window, &rect);
	_width = rect.right;
	_height = rect.bottom;
	_scale = 1.f;
	vsync(true);
	ShowWindow(window, SW_SHOW);
}

WindowWin32::~WindowWin32()
{
	wglMakeCurrent(_dc, NULL);
	ReleaseDC(_window, _dc);
	wglDeleteContext(_gl);
	DestroyWindow(_window);
}

WindowWin32* WindowWin32::create(WindowRenderApi api, int major, int minor)
{
	HWND window;
	HDC dc;
	HGLRC gl;

	init_opengl();
	window = create_window();
	dc = GetDC(window);
	gl = create_opengl(window, dc);
	return new WindowWin32(window, dc, gl);
}

void WindowWin32::swap()
{
	SwapBuffers(_dc);
}

void WindowWin32::poll(Input& input)
{
	MSG msg;
	BOOL ret;
	InputEvent e;

	for (;;)
	{
		ret = PeekMessageW(&msg, _window, 0, 0, PM_REMOVE);
		if (!ret)
			break;
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
		if (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP)
		{
			bool down;
			int keycode = 0;

			if (msg.message == WM_KEYDOWN)
				down = true;
			else
				down = false;

			switch (msg.wParam)
			{
			case VK_RETURN:
				keycode = Keyboard::Enter;
				break;
			case VK_ESCAPE:
				keycode = Keyboard::Escape;
				break;
			case VK_LEFT:
				keycode = Keyboard::Left;
				break;
			case VK_RIGHT:
				keycode = Keyboard::Right;
				break;
			case VK_UP:
				keycode = Keyboard::Up;
				break;
			case VK_DOWN:
				keycode = Keyboard::Down;
				break;
			case VK_SHIFT:
				keycode = Keyboard::Shift;
				break;
			}
			if (keycode)
			{
				e.type = down ? InputEventType::KeyDown : InputEventType::KeyUp;
				e.key.scancode = keycode;
				input.dispatch(e);
			}
		}
		else if (msg.message == WM_CHAR)
		{
			e.type = InputEventType::Text;
			e.text.unicode = msg.wParam;
			input.dispatch(e);
		}
	}
}

void WindowWin32::vsync(bool sync)
{
	wglSwapIntervalEXT(sync ? 1 : 0);
	Window::vsync(sync);
}
