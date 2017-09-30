#ifndef WINDOW_SDL2_H
#define WINDOW_SDL2_H

#include <SDL2/SDL.h>
#include <window/window.h>

class WindowSDL2 : public Window
{
public:
    WindowSDL2(SDL_Window* window, SDL_GLContext opengl);
    ~WindowSDL2();
    
    static WindowSDL2* create(WindowRenderApi api, int major, int minor);

    void    swap() override;
    void    poll(Keyboard& keyboard) override;

private:
    SDL_Window*     _window;
    SDL_GLContext   _opengl;
};

#endif
