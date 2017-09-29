#ifndef DRAW_BUFFER_H
#define DRAW_BUFFER_H

#include <math/vector.h>
#include <std/array.h>
#include <glyph.h>

class DrawBuffer
{
public:
    DrawBuffer();
    ~DrawBuffer();

    Vector2u size() const { return _size; }
    uint32_t width() const { return _size.x; }
    uint32_t height() const { return _size.y; }

    Glyph at(size_t index) const
    {
        Glyph g;

        g.symbol = _symbol[index];
        g.color.r = _color[index * 4 + 0];
        g.color.g = _color[index * 4 + 1];
        g.color.b = _color[index * 4 + 2];
        g.color_bg.r = _color_bg[index * 4 + 0];
        g.color_bg.g = _color_bg[index * 4 + 1];
        g.color_bg.b = _color_bg[index * 4 + 2];

        return g;
    }

    Glyph at(uint32_t x, uint32_t y) const { return at(x + y * _size.x); }

    void set(uint32_t x, uint32_t y, Glyph glyph)
    {
        size_t index;

        index = x + y * _size.x;
        _symbol[index] = glyph.symbol;
        _color[index * 4 + 0] = glyph.color.r;
        _color[index * 4 + 1] = glyph.color.g;
        _color[index * 4 + 2] = glyph.color.b;
        _color_bg[index * 4 + 0] = glyph.color_bg.r;
        _color_bg[index * 4 + 1] = glyph.color_bg.g;
        _color_bg[index * 4 + 2] = glyph.color_bg.b;
    }

    void resize(uint32_t width, uint32_t height);
    void clear();

    const uint16_t*     symbol() const { return _symbol.data(); }
    const uint8_t*      color() const { return _color.data(); }
    const uint8_t*      color_bg() const { return _color_bg.data(); }

private:
    Vector2u        _size;
    Array<uint16_t> _symbol;
    Array<uint8_t>  _color;
    Array<uint8_t>  _color_bg;
};

void putchar(DrawBuffer& draw_buffer, int x, int y, uint16_t symbol, Color color, Color color_bg);
void putchar_fast(DrawBuffer& draw_buffer, int x, int y, uint16_t symbol, Color color, Color color_bg);
void print(DrawBuffer& draw_buffer, int x, int y, const char* str, Color color, Color color_bg);
void printf(DrawBuffer& draw_buffer, int x, int y, const char* format, Color color, Color color_bg, ...);

#endif
