#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL2/SDL.h>
#include <non_copyable.h>
#include <cstdint>
#include <bitset>
#include <vector>

class Keyboard : private NonCopyable
{
public:
    Keyboard();
    ~Keyboard();

    bool    down(uint32_t keycode) const { return _down[keycode]; }
    bool    pressed(uint32_t keycode) const { return _pressed[keycode]; }
    bool    pressed_repeat(uint32_t keycode) const;

    void    tick();
    void    press_key(uint32_t keycode);
    void    release_key(uint32_t keycode);

private:
    uint32_t                        _tick;
    std::bitset<SDL_NUM_SCANCODES>  _down;
    std::bitset<SDL_NUM_SCANCODES>  _pressed;
    uint32_t                        _pressed_ticks[SDL_NUM_SCANCODES];
};

#endif
