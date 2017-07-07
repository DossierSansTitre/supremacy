#include <algorithm>
#include <config.h>
#include <renderer.h>

static uint16_t float16(float in)
{
    uint32_t inu = *((uint32_t*)&in);
    uint16_t e;
    uint16_t m;

    e = (uint16_t)((inu & 0x7f800000) >> 23);
    if (e == 0u)
        return 0;
    e -= 112;
    m = (uint16_t)((inu & 0x7fffff) >> 13);
    return ((e << 10) | m);
}

Renderer::Renderer(ThreadPool& thread_pool)
: _thread_pool(thread_pool)
{
    _texture = load_texture(SUPREMACY_DATA_PATH "/tileset.bmp", _texture_width, _texture_height);
    _tile_width = _texture_width / 32;
    _tile_height = _texture_height / 32;

    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ibo);
}

Renderer::~Renderer()
{

}

void Renderer::resize(Vector2u size)
{
    _size = size;
    _tiles_x = size.x;
    _tiles_y = size.y;

    build_indices();
    build_vertices();
}

void Renderer::render(const DrawBuffer& db)
{
    static const int factor = 12;

    int render_task;

    if (db.size() != _size)
        resize(db.size());
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

    glBufferData(GL_ARRAY_BUFFER, _tiles_x * _tiles_y * 4 * sizeof(Vertex), nullptr, GL_STREAM_DRAW);

    render_task = _thread_pool.task_create();
    for (size_t i = 0; i < _tiles_y / factor; ++i)
    {
        _thread_pool.task_perform(render_task, std::bind(&Renderer::render_lines, this, std::ref(db), i * factor, factor));
    }
    _thread_pool.task_wait(render_task);

    glBufferSubData(GL_ARRAY_BUFFER, 0, _tiles_x * _tiles_y * 4 * sizeof(Vertex), _vertices.data());

    glBindTexture(GL_TEXTURE_2D, _texture);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glVertexPointer(2, GL_SHORT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, x));
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color_bg_r));
    glDrawElements(GL_TRIANGLES, _tiles_x * _tiles_y * 6, GL_UNSIGNED_INT, (GLvoid*)0);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_HALF_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texture_x));
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color_r));
    glDrawElements(GL_TRIANGLES, _tiles_x * _tiles_y * 6, GL_UNSIGNED_INT, (GLvoid*)0);
}

void Renderer::render_lines(const DrawBuffer& db, uint32_t base, uint32_t count)
{
    uint32_t y;

    for (uint32_t j = 0; j < count; ++j)
    {
        y = base + j;
        for (uint32_t x = 0; x < _tiles_x; ++x)
        {
            render_tile(db, x, y);
        }
    }
}

void Renderer::render_tile(const DrawBuffer& db, uint32_t x, uint32_t y)
{
    static const float texture_size = 1.f / 32.f;

    Glyph       glyph;
    size_t      index;
    float       tx0;
    float       tx1;
    float       ty0;
    float       ty1;

    index = x + y * _tiles_x;
    glyph = db.at(x, y);

    tx0 = (glyph.symbol % 32) * texture_size;
    ty0 = (glyph.symbol / 32) * texture_size;
    tx1 = tx0 + texture_size;
    ty1 = ty0 + texture_size;

    render_vertex(index, 0, x, y, tx0, ty0, glyph.color, glyph.color_bg);
    render_vertex(index, 1, x + 1, y, tx1, ty0, glyph.color, glyph.color_bg);
    render_vertex(index, 2, x + 1, y + 1, tx1, ty1, glyph.color, glyph.color_bg);
    render_vertex(index, 3, x, y + 1, tx0, ty1, glyph.color, glyph.color_bg);
}

void Renderer::render_vertex(size_t index, size_t sub_index, uint32_t x, uint32_t y, float tx, float ty, Color color, Color color_bg)
{
    Vertex& v = _vertices[index * 4 + sub_index];

    v.x = x * _tile_width;
    v.y = y * _tile_height;
    v.texture_x = float16(tx);
    v.texture_y = float16(ty);
    v.color_r = color.r;
    v.color_g = color.g;
    v.color_b = color.b;
    v.color_bg_r = color_bg.r;
    v.color_bg_g = color_bg.g;
    v.color_bg_b = color_bg.b;
}

void Renderer::build_indices()
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

void Renderer::build_vertices()
{
    size_t count;

    count = _tiles_x * _tiles_y;
    _vertices.resize(count * 4);
}
