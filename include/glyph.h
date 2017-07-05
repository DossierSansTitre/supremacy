#ifndef GLYPH_H
#define GLYPH_H

#include <cstdint>
#include <color.h>

struct Glyph
{
    uint16_t    symbol;
    Color       color;
    Color       color_bg;
};

#endif
