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
    friend class Input;

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

    bool down(uint32_t scancode) const { return _down[scancode]; }

private:
    std::bitset<128>    _down;
};

#endif
