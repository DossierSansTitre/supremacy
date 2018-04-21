#include <algorithm>
#include <config.h>
#include <rendering/renderer_opengl_legacy.h>
#include <util/file_path.h>
#include <window/window.h>
#include <opengl.h>
#include <log.h>

static uint16_t float16(float in)
{
    uint32_t inu;
    uint16_t e;
    uint16_t m;

    memcpy((char*)&inu, (char*)&in, 4);
    e = (uint16_t)((inu & 0x7f800000) >> 23);
    if (e == 0u)
        return 0;
    e -= 112;
    m = (uint16_t)((inu & 0x7fffff) >> 13);
    return ((e << 10) | m);
}

RendererOpenGLLegacy::RendererOpenGLLegacy(Window& window, DrawBuffer& draw_buffer)
: Renderer(window, draw_buffer)
, _dirty(true)
{
    log_line(LogLevel::Info, "Using the OpenGL Legacy Renderer");
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, window.width(), window.height(), 0, 1, -1);
    glMatrixMode(GL_MODELVIEW);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glColor4f(1.f, 1.f, 1.f, 1.f);

    _texture = load_texture("tileset.png", &_texture_width, &_texture_height);
    _tile_width = _texture_width / 32;
    _tile_height = _texture_height / 32;

    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_vbo_static);
    glGenBuffers(1, &_ibo);
}

RendererOpenGLLegacy::~RendererOpenGLLegacy()
{

}

void RendererOpenGLLegacy::resize(Vector2u size)
{
    _size = size;
    _tiles_x = size.x;
    _tiles_y = size.y;

    build_indices();
    build_vertices();
}

void RendererOpenGLLegacy::clear()
{
    _dirty = true;
    _draw_buffer.resize(_window.width() / _tile_width, _window.height() / _tile_height);
}

void RendererOpenGLLegacy::render()
{
    if (_draw_buffer.size() != _size)
        resize(_draw_buffer.size());
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_static);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_SHORT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

    if (_dirty)
    {
        glBufferData(GL_ARRAY_BUFFER, _tiles_x * _tiles_y * 4 * sizeof(Vertex), nullptr, GL_STREAM_DRAW);
        render_tiles();
        glBufferSubData(GL_ARRAY_BUFFER, 0, _tiles_x * _tiles_y * 4 * sizeof(Vertex), _vertices.data());
    }

    glBindTexture(GL_TEXTURE_2D, _texture);
    glEnableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color_bg_r));
    glDrawElements(GL_TRIANGLES, _tiles_x * _tiles_y * 6, GL_UNSIGNED_INT, (GLvoid*)0);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_HALF_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texture_x));
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color_r));
    glDrawElements(GL_TRIANGLES, _tiles_x * _tiles_y * 6, GL_UNSIGNED_INT, (GLvoid*)0);
    _dirty = false;
}

void RendererOpenGLLegacy::render_tiles()
{
    for (size_t y = 0; y < _tiles_y; ++y)
    {
        for (size_t x = 0; x < _tiles_x; ++x)
        {
            render_tile(x, y);
        }
    }
}

void RendererOpenGLLegacy::render_tile(size_t x, size_t y)
{
    static const float texture_size = 1.f / 32.f;

    Glyph       glyph;
    float       tx[2];
    float       ty[2];
    uint16_t    t16x[2];
    uint16_t    t16y[2];
    size_t index;

    index = x + y * _tiles_x;
    glyph = _draw_buffer.at(x, y);

    tx[0] = (glyph.symbol % 32) * texture_size;
    ty[0] = (glyph.symbol / 32) * texture_size;
    tx[1] = tx[0] + texture_size;
    ty[1] = ty[0] + texture_size;

    t16x[0] = float16(tx[0]);
    t16x[1] = float16(tx[1]);
    t16y[0] = float16(ty[0]);
    t16y[1] = float16(ty[1]);

    render_vertex(index, 0, t16x[0], t16y[0], glyph.color, glyph.color_bg);
    render_vertex(index, 1, t16x[1], t16y[0], glyph.color, glyph.color_bg);
    render_vertex(index, 2, t16x[1], t16y[1], glyph.color, glyph.color_bg);
    render_vertex(index, 3, t16x[0], t16y[1], glyph.color, glyph.color_bg);
}

void RendererOpenGLLegacy::render_vertex(size_t index, size_t sub_index, uint16_t tx, uint16_t ty, Color color, Color color_bg)
{
    Vertex& v = _vertices[index * 4 + sub_index];

    v.texture_x = tx;
    v.texture_y = ty;
    v.color_r = color.r;
    v.color_g = color.g;
    v.color_b = color.b;
    v.color_bg_r = color_bg.r;
    v.color_bg_g = color_bg.g;
    v.color_bg_b = color_bg.b;
}

void RendererOpenGLLegacy::build_indices()
{
    size_t indice_count;
    uint32_t* data;

    indice_count = _tiles_x * _tiles_y;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indice_count * 6 * 4, nullptr, GL_STATIC_DRAW);
    data = (uint32_t*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);

    for (uint32_t i = 0; i < indice_count; ++i)
    {
        data[i * 6 + 0] = i * 4 + 0;
        data[i * 6 + 1] = i * 4 + 1;
        data[i * 6 + 2] = i * 4 + 2;
        data[i * 6 + 3] = i * 4 + 0;
        data[i * 6 + 4] = i * 4 + 2;
        data[i * 6 + 5] = i * 4 + 3;
    }

    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

void RendererOpenGLLegacy::build_vertices()
{
    size_t count;
    int16_t* data;

    count = _tiles_x * _tiles_y;
    _vertices.resize(count * 4);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_static);
    glBufferData(GL_ARRAY_BUFFER, count * 8 * sizeof(int16_t), nullptr, GL_STATIC_DRAW);
    data = (int16_t*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    for (size_t i = 0; i < count; ++i)
    {
        data[i * 8 + 0] = (i % _tiles_x + 0) * _tile_width;
        data[i * 8 + 1] = (i / _tiles_x + 0) * _tile_height;
        data[i * 8 + 2] = (i % _tiles_x + 1) * _tile_width;
        data[i * 8 + 3] = (i / _tiles_x + 0) * _tile_height;
        data[i * 8 + 4] = (i % _tiles_x + 1) * _tile_width;
        data[i * 8 + 5] = (i / _tiles_x + 1) * _tile_height;
        data[i * 8 + 6] = (i % _tiles_x + 0) * _tile_width;
        data[i * 8 + 7] = (i / _tiles_x + 1) * _tile_height;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
}
