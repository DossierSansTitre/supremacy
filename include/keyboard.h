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

    enum
    {
        Unknown = 0x00,
        Enter   = 0x01,
        Escape  = 0x02,
        Right   = 0x03,
        Left    = 0x04,
        Down    = 0x05,
        Up      = 0x06,
        Shift   = 0x07
    };

    bool    down(uint32_t scancode) const { return _scancode_down[scancode]; }
    bool    pressed(uint32_t scancode) const { return _scancode_pressed[scancode]; }
    bool    repeated(uint32_t scancode) const;

    bool    key_down(SDL_Keycode keycode) const { return false; }
    bool    key_pressed(SDL_Keycode keycode) const { return false; }
    bool    key_repeated(SDL_Keycode keycode) const { return false; }

    void    tick();

    void    set_scancode(uint32_t scancode, bool down);

private:
    uint32_t                        _tick;
    std::bitset<128>                _scancode_down;
    std::bitset<128>                _scancode_pressed;
    uint32_t                        _scancode_ticks[128];
};

#endif
