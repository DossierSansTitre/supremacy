#include <keyboard.h>

Keyboard::Keyboard()
: _tick(0u)
{

}

Keyboard::~Keyboard()
{

}

bool Keyboard::repeated(uint32_t scancode) const
{
    static const uint32_t delay_initial = 8;
    static const uint32_t delay = 2;

    uint32_t key_tick;
    uint32_t delta;

    if (_scancode_pressed[scancode])
        return true;
    if (_scancode_down[scancode])
    {
        key_tick = _scancode_ticks[scancode];
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
    _scancode_pressed.reset();
}

void Keyboard::set_scancode(uint32_t scancode, bool down)
{
    if (scancode >= 0x80)
        return;
    if (down)
    {
        _scancode_down[scancode] = true;
        _scancode_pressed[scancode] = true;
        _scancode_ticks[scancode] = _tick;
    }
    else
    {
        _scancode_down[scancode] = false;
    }
}
