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
    size_t size;

    _size.x = width;
    _size.y = height;
    size = width * height;
    _symbol.resize(size);
    _color.resize(size * 4);
    _color_bg.resize(size * 4);
    clear();
}

void DrawBuffer::clear()
{
    size_t size;

    size = _size.x * _size.y;
    std::memset(_symbol.data(), 0, size);
    std::memset(_color.data(), 0, size * 4);
    std::memset(_color_bg.data(), 0, size * 4);
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
