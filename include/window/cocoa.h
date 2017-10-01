#ifndef WINDOW_COCOA_H
#define WINDOW_COCOA_H

#include <window/window.h>

class WindowCocoa : public Window
{
public:
    WindowCocoa(void* window, void* context);
    ~WindowCocoa();

    static WindowCocoa* create(WindowRenderApi api, int major, int minor);

    void    swap() override;
    void    poll(Keyboard& keyboard) override;

private:
    void*   _window;
    void*   _context;
};

#endif
