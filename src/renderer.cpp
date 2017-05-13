#include <algorithm>
#include <config.h>
#include <renderer.h>

Renderer::Renderer()
: _texture(0)
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
    std::fill(_symbols.begin(), _symbols.end(), 0);
}

void Renderer::destroy()
{

}

void Renderer::putchar(int i, uint16_t symbol, Color color, Color color_bg)
{
    if (i == -1)
        return;
    _symbols[i] = symbol;
    _colors[i] = color;
    _colors_bg[i] = color_bg;
}

void Renderer::putchar(int x, int y, uint16_t symbol, Color color, Color color_bg)
{
    putchar(index(x, y), symbol, color, color_bg);
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
    static const float texture_size = 1.f / 32.f;

    uint16_t    symbol;
    Color       color;
    Color       color_bg;
    size_t      i;
    float       tx;
    float       ty;
    Vertex*     data;

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

    glBufferData(GL_ARRAY_BUFFER, _tiles_x * _tiles_y * 4 * sizeof(Vertex), nullptr, GL_STREAM_DRAW);
    data = _vertices.data();
    for (uint32_t y = 0; y < _tiles_y; ++y)
    {
        for (uint32_t x = 0; x < _tiles_x; ++x)
        {
            i = x + y * _tiles_x;
            symbol = _symbols[i];
            color = _colors[i];
            color_bg = _colors_bg[i];

            tx = (symbol % 32) * texture_size;
            ty = (symbol / 32) * texture_size;

            data[i * 4 + 0].x = static_cast<float>(x * _tile_width);
            data[i * 4 + 0].y = static_cast<float>(y * _tile_height);
            data[i * 4 + 0].texture_x = tx;
            data[i * 4 + 0].texture_y = ty;
            data[i * 4 + 0].color_r = color.r;
            data[i * 4 + 0].color_g = color.g;
            data[i * 4 + 0].color_b = color.b;
            data[i * 4 + 0].color_bg_r = color_bg.r;
            data[i * 4 + 0].color_bg_g = color_bg.g;
            data[i * 4 + 0].color_bg_b = color_bg.b;

            data[i * 4 + 1].x = static_cast<float>((x + 1) * _tile_width);
            data[i * 4 + 1].y = static_cast<float>(y * _tile_height);
            data[i * 4 + 1].texture_x = tx + texture_size;
            data[i * 4 + 1].texture_y = ty;
            data[i * 4 + 1].color_r = color.r;
            data[i * 4 + 1].color_g = color.g;
            data[i * 4 + 1].color_b = color.b;
            data[i * 4 + 1].color_bg_r = color_bg.r;
            data[i * 4 + 1].color_bg_g = color_bg.g;
            data[i * 4 + 1].color_bg_b = color_bg.b;

            data[i * 4 + 2].x = static_cast<float>((x + 1) * _tile_width);
            data[i * 4 + 2].y = static_cast<float>((y + 1) * _tile_height);
            data[i * 4 + 2].texture_x = tx + texture_size;
            data[i * 4 + 2].texture_y = ty + texture_size;
            data[i * 4 + 2].color_r = color.r;
            data[i * 4 + 2].color_g = color.g;
            data[i * 4 + 2].color_b = color.b;
            data[i * 4 + 2].color_bg_r = color_bg.r;
            data[i * 4 + 2].color_bg_g = color_bg.g;
            data[i * 4 + 2].color_bg_b = color_bg.b;

            data[i * 4 + 3].x = static_cast<float>(x * _tile_width);
            data[i * 4 + 3].y = static_cast<float>((y + 1) * _tile_height);
            data[i * 4 + 3].texture_x = tx;
            data[i * 4 + 3].texture_y = ty + texture_size;
            data[i * 4 + 3].color_r = color.r;
            data[i * 4 + 3].color_g = color.g;
            data[i * 4 + 3].color_b = color.b;
            data[i * 4 + 3].color_bg_r = color_bg.r;
            data[i * 4 + 3].color_bg_g = color_bg.g;
            data[i * 4 + 3].color_bg_b = color_bg.b;
        }
    }
    glBufferSubData(GL_ARRAY_BUFFER, 0, _tiles_x * _tiles_y * 4 * sizeof(Vertex), data);

    glBindTexture(GL_TEXTURE_2D, _texture);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, x));
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color_bg_r));
    glDrawElements(GL_TRIANGLES, _tiles_x * _tiles_y * 6, GL_UNSIGNED_INT, (GLvoid*)0);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texture_x));
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color_r));
    glDrawElements(GL_TRIANGLES, _tiles_x * _tiles_y * 6, GL_UNSIGNED_INT, (GLvoid*)0);
}

int Renderer::index(int x, int y) const
{
    if (x < 0 || x >= (int)_tiles_x || y < 0 || y >= (int)_tiles_y)
        return -1;
    return x + y * _tiles_x;
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
