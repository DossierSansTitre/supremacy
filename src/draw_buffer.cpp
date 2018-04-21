#include <cstdarg>
#include <cstdio>
#include <cstring>
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
    _row_size_color = ((width * 4 + 7) / 8) * 8;
    _row_size_symbol = ((width + 3) / 4) * 4;
    _symbol.resize(_row_size_symbol * height);
    _color.resize(_row_size_color * height);
    _color_bg.resize(_row_size_color * height);
    clear();
}

void DrawBuffer::clear()
{
    std::memset(_symbol.data(), 0, _row_size_symbol * _size.y);
    std::memset(_color.data(), 0, _row_size_color * _size.y);
    std::memset(_color_bg.data(), 0, _row_size_color * _size.y);
}

void putchar(DrawBuffer& draw_buffer, int x, int y, uint16_t symbol, Color color, Color color_bg)
{
    if (x < 0 || x >= (int)draw_buffer.width() || y < 0 || y >= (int)draw_buffer.height())
        return;
    putchar_fast(draw_buffer, x, y, symbol, color, color_bg);
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
