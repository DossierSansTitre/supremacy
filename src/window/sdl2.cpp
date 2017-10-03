#include <iostream>
#include <opengl.h>
#include <window/sdl2.h>
#include <cli_options.h>
#include <keyboard.h>

WindowSDL2::WindowSDL2(SDL_Window* window, SDL_GLContext opengl)
: _window(window)
, _opengl(opengl)
{
    int w;
    int h;

    SDL_GetWindowSize(_window, &w, &h);
    this->_width = w;
    this->_height = h;
    vsync(true);
    SDL_StartTextInput();
}

WindowSDL2::~WindowSDL2()
{
    SDL_GL_MakeCurrent(_window, nullptr);
    SDL_GL_DeleteContext(_opengl);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

static SDL_Window* create_window()
{
    SDL_Window* window;
    SDL_DisplayMode dm;
    uint32_t sdl_flags;
    const auto& opts = CliOptions::instance();

    SDL_GetDesktopDisplayMode(0, &dm);
    sdl_flags = SDL_WINDOW_OPENGL;
    if (!opts.windowed)
        sdl_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    window = SDL_CreateWindow(
        "Supremacy",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        dm.w,
        dm.h,
        sdl_flags
    );
    return window;
}

static SDL_GLContext create_gl_context(SDL_Window* window)
{
    SDL_GLContext opengl;

    opengl = SDL_GL_CreateContext(window);
    if (opengl == nullptr)
        return nullptr;
    glewInit();
    return opengl;
}

WindowSDL2* WindowSDL2::create(WindowRenderApi api, int major, int minor)
{
    if (api != WindowRenderApi::OpenGL)
        return nullptr;

    SDL_GLprofile gl_profile;
    SDL_Window* window;
    SDL_GLContext opengl;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    if (major >= 3)
        gl_profile = SDL_GL_CONTEXT_PROFILE_CORE;
    else
        gl_profile = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, gl_profile);
    window = create_window();
    if (window == nullptr)
    {
        SDL_Quit();
        return nullptr;
    }
    opengl = create_gl_context(window);
    if (opengl == nullptr)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return nullptr;
    }
    return new WindowSDL2(window, opengl);
}

void WindowSDL2::swap()
{
    SDL_GL_SwapWindow(_window);
}

void WindowSDL2::poll(Input& input)
{
    InputEvent e;
    SDL_Event event;
    uint32_t scancode;
    uint32_t unicode;
    bool down;

    while (SDL_PollEvent(&event))
    {
        down = (event.type == SDL_KEYDOWN);
        if (down || event.type == SDL_KEYUP)
        {
            scancode = 0;
            switch (event.key.keysym.scancode)
            {
                case SDL_SCANCODE_ESCAPE:
                    scancode = Keyboard::Escape;
                    break;
                case SDL_SCANCODE_RETURN:
                    scancode = Keyboard::Enter;
                    break;
                case SDL_SCANCODE_LSHIFT:
                    scancode = Keyboard::Shift;
                    break;
                case SDL_SCANCODE_LEFT:
                    scancode = Keyboard::Left;
                    break;
                case SDL_SCANCODE_RIGHT:
                    scancode = Keyboard::Right;
                    break;
                case SDL_SCANCODE_UP:
                    scancode = Keyboard::Up;
                    break;
                case SDL_SCANCODE_DOWN:
                    scancode = Keyboard::Down;
                    break;
                default:
                    break;
            }
            if (!scancode)
                continue;
            e.type = down ? InputEventType::KeyDown : InputEventType::KeyUp;
            e.key.scancode = scancode;
            input.dispatch(e);
        }
        else if (event.type == SDL_TEXTINPUT)
        {
            unicode = event.text.text[0];
            if (!unicode || unicode > 0x7f)
                continue;
            e.type = InputEventType::Text;
            e.text.unicode = unicode;
            input.dispatch(e);
        }
    }
}

void WindowSDL2::vsync(bool sync)
{
    int ret;

    if (sync)
    {
        ret = SDL_GL_SetSwapInterval(-1);
        if (ret == -1)
            SDL_GL_SetSwapInterval(1);
    }
    else
    {
        SDL_GL_SetSwapInterval(0);
    }
    Window::vsync(sync);
}
