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

    bool    down(SDL_Scancode scancode) const { return _down[scancode]; }
    bool    pressed(SDL_Scancode scancode) const { return _pressed[scancode]; }
    bool    repeated(SDL_Scancode scancode) const;

    bool    key_down(SDL_Keycode keycode) const { return down(SDL_GetScancodeFromKey(keycode)); }
    bool    key_pressed(SDL_Keycode keycode) const { return pressed(SDL_GetScancodeFromKey(keycode)); }
    bool    key_repeated(SDL_Keycode keycode) const { return repeated(SDL_GetScancodeFromKey(keycode)); }

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
