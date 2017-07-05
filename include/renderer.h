#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <non_copyable.h>
#include <opengl.h>
#include <color.h>
#include <thread_pool.h>
#include <draw_buffer.h>
#include <math/vector.h>

class Renderer : private NonCopyable
{
public:
    Renderer(ThreadPool& thread_pool);
    ~Renderer();

    void render(const DrawBuffer& draw_buffer);

private:
    void render_lines(const DrawBuffer& db, uint32_t base, uint32_t count);
    void render_tile(const DrawBuffer& db, uint32_t x, uint32_t y);
    void render_vertex(size_t index, size_t sub_index, uint32_t x, uint32_t y, float tx, float ty, Color color, Color color_bg);

    struct Vertex {
        int16_t     x;
        int16_t     y;
        uint16_t    texture_x;
        uint16_t    texture_y;
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

    void build_indices();
    void build_vertices();
    void resize(Vector2u size);

    ThreadPool& _thread_pool;

    GLuint _texture;
    GLuint _vbo;
    GLuint _ibo;

    Vector2u    _size;
    uint32_t    _texture_width;
    uint32_t    _texture_height;
    uint32_t    _tile_width;
    uint32_t    _tile_height;
    uint32_t    _tiles_x;
    uint32_t    _tiles_y;

    std::vector<Vertex> _vertices;
};

#endif
