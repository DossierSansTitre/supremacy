#include <algorithm>
#include <config.h>
#include <renderer.h>

Renderer::Renderer(ThreadPool& thread_pool)
: _thread_pool(thread_pool)
, _texture(0)
, _vbo(0)
, _ibo(0)
{

}

Renderer::~Renderer()
{

}

void Renderer::init(uint32_t width, uint32_t height)
{
    _texture = load_texture(SUPREMACY_DATA_PATH "/tileset.bmp", _texture_width, _texture_height);
    _tile_width = _texture_width / 32;
    _tile_height = _texture_height / 32;

    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ibo);
    resize(width, height);
}

void Renderer::resize(uint32_t width, uint32_t height)
{
    size_t size;

    _tiles_x = width / _tile_width;
    _tiles_y = height / _tile_height;

    size = _tiles_x * _tiles_y;

    _symbols.resize(size);
    _colors.resize(size);
    _colors_bg.resize(size);

    clear();
    build_indices();
    build_vertices();
}

void Renderer::clear()
{
    Color black{0, 0, 0};
    std::fill(_symbols.begin(), _symbols.end(), 0);
    std::fill(_colors_bg.begin(), _colors_bg.end(), black);
}

void Renderer::destroy()
{

}

void Renderer::print(int x, int y, const char* str, Color color, Color color_bg)
{
    int len;

    len = static_cast<int>(strlen(str));
    for (int i = 0; i < len; ++i)
    {
        putchar(x + i, y, str[i], color, color_bg);
    }
}

void Renderer::printf(int x, int y, const char* str, Color color, Color color_bg, ...)
{
    static char buffer[4096];
    va_list ap;

    va_start(ap, color_bg);
    vsnprintf(buffer, 4096, str, ap);
    va_end(ap);
    print(x, y, buffer, color, color_bg);
}

/*
 * We use a (somewhat) complex arch to speed up rendering.
 * Everything is batched into one VBO.
 * A vertex is defined by:
 * Position: 2 floats (8 bytes)
 * Texture: 2 floats(8 bytes)
 * Color:   3 u8 and padding (4 bytes)
 * Color2:  3 u8 and padding (4 bytes)
 *
 * Each vertex takes 24 bytes, for a total of 96 bytes per quad.
 */
void Renderer::render()
{
    int render_task;

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

    glBufferData(GL_ARRAY_BUFFER, _tiles_x * _tiles_y * 4 * sizeof(Vertex), nullptr, GL_STREAM_DRAW);

    size_t thread_count = _thread_pool.pool_size();
    size_t lines_per_thread = ceil((float)_tiles_y / thread_count);
    size_t base;
    size_t count;

    render_task = _thread_pool.task_create();

    for (size_t i = 0; i < thread_count - 1; ++i)
    {
        base = i * lines_per_thread;
        count = lines_per_thread;
        _thread_pool.task_perform(render_task, std::bind(&Renderer::render_lines, this, base, count));
    }
    base = (thread_count - 1) * lines_per_thread;
    count = _tiles_y - base;
    _thread_pool.task_perform(render_task, std::bind(&Renderer::render_lines, this, base, count));

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
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texture_x));
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color_r));
    glDrawElements(GL_TRIANGLES, _tiles_x * _tiles_y * 6, GL_UNSIGNED_INT, (GLvoid*)0);
}

void Renderer::render_lines(uint32_t base, uint32_t count)
{
    static const float texture_size = 1.f / 32.f;

    uint32_t    y;
    uint16_t    symbol;
    Color       color;
    Color       color_bg;
    size_t      index;
    float       tx;
    float       ty;
    Vertex*     data;

    data = _vertices.data();
    for (uint32_t j = 0; j < count; ++j)
    {
        y = base + j;
        for (uint32_t x = 0; x < _tiles_x; ++x)
        {
            index = x + y * _tiles_x;
            symbol = _symbols[index];
            color = _colors[index];
            color_bg = _colors_bg[index];

            tx = (symbol % 32) * texture_size;
            ty = (symbol / 32) * texture_size;

            data[index * 4 + 0].x = x * _tile_width;
            data[index * 4 + 0].y = y * _tile_height;
            data[index * 4 + 0].texture_x = tx;
            data[index * 4 + 0].texture_y = ty;
            data[index * 4 + 0].color_r = color.r;
            data[index * 4 + 0].color_g = color.g;
            data[index * 4 + 0].color_b = color.b;
            data[index * 4 + 0].color_bg_r = color_bg.r;
            data[index * 4 + 0].color_bg_g = color_bg.g;
            data[index * 4 + 0].color_bg_b = color_bg.b;

            data[index * 4 + 1].x = (x + 1) * _tile_width;
            data[index * 4 + 1].y = y * _tile_height;
            data[index * 4 + 1].texture_x = tx + texture_size;
            data[index * 4 + 1].texture_y = ty;
            data[index * 4 + 1].color_r = color.r;
            data[index * 4 + 1].color_g = color.g;
            data[index * 4 + 1].color_b = color.b;
            data[index * 4 + 1].color_bg_r = color_bg.r;
            data[index * 4 + 1].color_bg_g = color_bg.g;
            data[index * 4 + 1].color_bg_b = color_bg.b;

            data[index * 4 + 2].x = (x + 1) * _tile_width;
            data[index * 4 + 2].y = (y + 1) * _tile_height;
            data[index * 4 + 2].texture_x = tx + texture_size;
            data[index * 4 + 2].texture_y = ty + texture_size;
            data[index * 4 + 2].color_r = color.r;
            data[index * 4 + 2].color_g = color.g;
            data[index * 4 + 2].color_b = color.b;
            data[index * 4 + 2].color_bg_r = color_bg.r;
            data[index * 4 + 2].color_bg_g = color_bg.g;
            data[index * 4 + 2].color_bg_b = color_bg.b;

            data[index * 4 + 3].x = x * _tile_width;
            data[index * 4 + 3].y = (y + 1) * _tile_height;
            data[index * 4 + 3].texture_x = tx;
            data[index * 4 + 3].texture_y = ty + texture_size;
            data[index * 4 + 3].color_r = color.r;
            data[index * 4 + 3].color_g = color.g;
            data[index * 4 + 3].color_b = color.b;
            data[index * 4 + 3].color_bg_r = color_bg.r;
            data[index * 4 + 3].color_bg_g = color_bg.g;
            data[index * 4 + 3].color_bg_b = color_bg.b;
        }
    }
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
