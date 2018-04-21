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

    Glyph at(uint32_t x, uint32_t y) const
    {
        size_t index_symbol;
        size_t index_color;
        Glyph g;

        index_symbol = y * _row_size_symbol + x;
        index_color = y * _row_size_color + x * 8;
        g.symbol = _symbol[index_symbol];
        g.color.b = _color[index_color + 0];
        g.color.g = _color[index_color + 1];
        g.color.r = _color[index_color + 2];
        g.color_bg.b = _color[index_color + 4];
        g.color_bg.g = _color[index_color + 5];
        g.color_bg.r = _color[index_color + 6];

        return g;
    }


    void set(uint32_t x, uint32_t y, Glyph glyph)
    {
        size_t index_symbol;
        size_t index_color;

        index_symbol = y * _row_size_symbol + x;
        index_color = y * _row_size_color + x * 8;
        _symbol[index_symbol] = glyph.symbol;
        _color[index_color + 0] = glyph.color.b;
        _color[index_color + 1] = glyph.color.g;
        _color[index_color + 2] = glyph.color.r;
        _color[index_color + 4] = glyph.color_bg.b;
        _color[index_color + 5] = glyph.color_bg.g;
        _color[index_color + 6] = glyph.color_bg.r;
    }

    void resize(uint32_t width, uint32_t height);
    void clear();

    const uint16_t*     symbol() const { return _symbol.data(); }
    const uint8_t*      color() const { return _color.data(); }

private:
    Vector2u        _size;
    size_t          _row_size_color;
    size_t          _row_size_symbol;
    Array<uint16_t> _symbol;
    Array<uint8_t>  _color;
};

inline void putchar_fast(DrawBuffer& draw_buffer, int x, int y, uint16_t symbol, Color color, Color color_bg)
{
    Glyph g;

    g.symbol = symbol;
    g.color = color;
    g.color_bg = color_bg;
    draw_buffer.set(x, y, g);
}

void putchar(DrawBuffer& draw_buffer, int x, int y, uint16_t symbol, Color color, Color color_bg);
void print(DrawBuffer& draw_buffer, int x, int y, const char* str, Color color, Color color_bg);
void printf(DrawBuffer& draw_buffer, int x, int y, const char* format, Color color, Color color_bg, ...);

#endif
