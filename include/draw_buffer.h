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

    Glyph at(size_t index) const { return _glyphs[index]; }
    Glyph at(uint32_t x, uint32_t y) const { return at(x + y * _size.x); }

    void set(uint32_t x, uint32_t y, Glyph glyph) { _glyphs[x + y * _size.x] = glyph; }
    void resize(uint32_t width, uint32_t height);
    void clear();

private:
    Vector2u        _size;
    Array<Glyph>    _glyphs;
};

void putchar(DrawBuffer& draw_buffer, int x, int y, uint16_t symbol, Color color, Color color_bg);
void putchar_fast(DrawBuffer& draw_buffer, int x, int y, uint16_t symbol, Color color, Color color_bg);
void print(DrawBuffer& draw_buffer, int x, int y, const char* str, Color color, Color color_bg);
void printf(DrawBuffer& draw_buffer, int x, int y, const char* format, Color color, Color color_bg, ...);

#endif
