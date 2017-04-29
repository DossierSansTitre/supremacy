#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <non_copyable.h>
#include <opengl.h>
#include <color.h>

class Renderer : private NonCopyable
{
public:
    Renderer();
    ~Renderer();

    uint32_t width() const { return _tiles_x; }
    uint32_t height() const { return _tiles_y; }

    void init(uint32_t width, uint32_t height);
    void resize(uint32_t width, uint32_t height);
    void clear();
    void destroy();

    void putchar(int i, uint16_t symbol, Color color, Color color_bg);
    void putchar(int x, int y, uint16_t symbol, Color color, Color color_bg);
    void print(int x, int y, const char* str, Color color, Color color_bg);
    void printf(int x, int y, const char* format, Color color, Color color_bg, ...);

    void render();

private:
    struct Vertex {
        float       x;
        float       y;
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

    int                     index(int x, int y) const;
    void                    build_indices();
    void                    build_vertices();

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
