#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <non_copyable.h>
#include <opengl.h>
#include <color.h>
#include <thread_pool.h>
#include <draw_buffer.h>
#include <math/vector.h>

class Window;
class Renderer : private NonCopyable
{
public:
    Renderer(ThreadPool& thread_pool);
    ~Renderer();

    void clear(DrawBuffer& draw_buffer, const Window& window);
    void render(const DrawBuffer& draw_buffer);
    void render_lines(const DrawBuffer& db, size_t start, size_t length);

private:
    void render_tile(const DrawBuffer& db, size_t index);
    void render_vertex(size_t index, size_t sub_index, uint16_t tx, uint16_t ty, Color color, Color color_bg);

    struct Vertex {
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
    GLuint _vbo_static;
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
