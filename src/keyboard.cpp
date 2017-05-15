#include <keyboard.h>

Keyboard::Keyboard()
: _tick(0u)
{

}

Keyboard::~Keyboard()
{

}

bool Keyboard::repeated(SDL_Scancode scancode) const
{
    static const uint32_t delay_initial = 8;
    static const uint32_t delay = 2;

    uint32_t key_tick;
    uint32_t delta;

    if (_pressed[scancode])
        return true;
    if (_down[scancode])
    {
        key_tick = _pressed_ticks[scancode];
        delta = _tick - key_tick;
        if (delta < delay_initial)
            return false;
        if (((delta - delay_initial) % delay) == 0)
            return true;
    }
    return false;
}

void Keyboard::tick()
{
    _tick++;
    _pressed.reset();
}

void Keyboard::press_key(uint32_t keycode)
{
    if (!_down[keycode])
    {
        _down[keycode] = true;
        _pressed[keycode] = true;
        _pressed_ticks[keycode] = _tick;
    }
}

void Keyboard::release_key(uint32_t keycode)
{
    _down[keycode] = false;
}
