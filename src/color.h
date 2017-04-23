#ifndef COLOR_H
#define COLOR_H

#include <cstdint>

struct Color
{
    uint8_t     r;
    uint8_t     g;
    uint8_t     b;

    Color& operator*=(float factor)
    {
        r *= factor;
        g *= factor;
        b *= factor;

        return *this;
    }
};

inline Color operator*(Color color, float factor)
{
    color *= factor;
    return color;
}

#endif
