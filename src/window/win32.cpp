#include <window/win32.h>
#include <window/win32_opengl.h>
#include <cli_options.h>
#include <keyboard.h>

static ATOM window_class = 0;

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

WindowWin32::WindowWin32(HWND window)
: _window(window)
{
	RECT rect;

	GetClientRect(window, &rect);
	_width = rect.right;
	_height = rect.bottom;
	_scale = 1.f;
	//vsync(true);
	ShowWindow(window, SW_SHOW);
}

WindowWin32::~WindowWin32()
{
	DestroyWindow(_window);
}

WindowWin32* WindowWin32::create(WindowRenderApi api, int major, int minor)
{
	HWND window;

	switch (api)
	{
	case WindowRenderApi::OpenGL:
		WindowWin32OpenGL::init();
		window = create_window();
		return WindowWin32OpenGL::create(window, major, minor);
	default:
		return nullptr;
	}
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
			e.text.unicode = (uint32_t)msg.wParam;
			input.dispatch(e);
		}
	}
}