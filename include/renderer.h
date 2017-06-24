#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <non_copyable.h>
#include <opengl.h>
#include <color.h>
#include <thread_pool.h>

class Renderer : private NonCopyable
{
public:
    Renderer(ThreadPool& thread_pool);
    ~Renderer();

    uint32_t width() const { return _tiles_x; }
    uint32_t height() const { return _tiles_y; }

    void init(uint32_t width, uint32_t height);
    void resize(uint32_t width, uint32_t height);
    void clear();
    void destroy();

    void putchar(int i, uint16_t symbol, Color color, Color color_bg)
    {
        if (i == -1)
            return;
        putchar_fast(i, symbol, color, color_bg);
    }

    void putchar(int x, int y, uint16_t symbol, Color color, Color color_bg)
    {
        putchar(index(x, y), symbol, color, color_bg);
    }

    void putchar_fast(int i, uint16_t symbol, Color color, Color color_bg)
    {
        _symbols[i] = symbol;
        _colors[i] = color;
        _colors_bg[i] = color_bg;
    }

    void putchar_fast(int x, int y, uint16_t symbol, Color color, Color color_bg)
    {
        putchar_fast(index_fast(x, y), symbol, color, color_bg);
    }

    void print(int x, int y, const char* str, Color color, Color color_bg);
    void printf(int x, int y, const char* format, Color color, Color color_bg, ...);

    void render();
    void render_lines(uint32_t base, uint32_t count);

private:
    struct Vertex {
        int16_t     x;
        int16_t     y;
        float       texture_x;
        float       texture_y;
        uint8_t     color_r;
        uint8_t     color_g;
        uint8_t     color_b;
        uint8_t     unused1;
        uint8_t     color_bg_r;
        uint8_t     color_bg_g;
        uint8_t     color_bg_b;
        uint8_t     unused2;
    };

    int index(int x, int y) const
    {
        if (x < 0 || x >= (int)_tiles_x || y < 0 || y >= (int)_tiles_y)
            return -1;
        return index_fast(x, y);
    }

    int index_fast(int x, int y) const
    {
        return x + y * _tiles_x;
    }

    void                    build_indices();
    void                    build_vertices();

    ThreadPool&             _thread_pool;

    GLuint                  _texture;
    GLuint                  _vbo;
    GLuint                  _ibo;

    uint32_t                _texture_width;
    uint32_t                _texture_height;
    uint32_t                _tile_width;
    uint32_t                _tile_height;
    uint32_t                _tiles_x;
    uint32_t                _tiles_y;

    std::vector<uint16_t>   _symbols;
    std::vector<Color>      _colors;
    std::vector<Color>      _colors_bg;

    std::vector<Vertex>     _vertices;
};

#endif
