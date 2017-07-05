#include <cstdarg>
#include <cstdio>
#include <draw_buffer.h>

DrawBuffer::DrawBuffer()
{

}

DrawBuffer::~DrawBuffer()
{

}

void DrawBuffer::resize(uint32_t width, uint32_t height)
{
    _size.x = width;
    _size.y = height;
    _glyphs.resize(width * height);
    clear();
}

void DrawBuffer::clear()
{
    for (auto& g : _glyphs)
    {
        g.symbol = ' ';
        g.color = {0, 0, 0};
        g.color_bg = {0, 0, 0};
    }
}

void putchar(DrawBuffer& draw_buffer, int x, int y, uint16_t symbol, Color color, Color color_bg)
{
    if (x < 0 || x >= draw_buffer.width() || y < 0 || y >= draw_buffer.height())
        return;
    putchar_fast(draw_buffer, x, y, symbol, color, color_bg);
}

void putchar_fast(DrawBuffer& draw_buffer, int x, int y, uint16_t symbol, Color color, Color color_bg)
{
    Glyph g;

    g.symbol = symbol;
    g.color = color;
    g.color_bg = color_bg;
    draw_buffer.set(x, y, g);
}

void print(DrawBuffer& draw_buffer, int x, int y, const char* str, Color color, Color color_bg)
{
    for (size_t i = 0; str[i]; ++i)
        putchar(draw_buffer, x + i, y, str[i], color, color_bg);
}

void printf(DrawBuffer& draw_buffer, int x, int y, const char* format, Color color, Color color_bg, ...)
{
    char buffer[2048];
    va_list va;

    va_start(va, color_bg);
    vsnprintf(buffer, 2048, format, va);
    va_end(va);
    print(draw_buffer, x, y, buffer, color, color_bg);
}
