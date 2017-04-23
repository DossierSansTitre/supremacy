#include <algorithm>
#include <config.h>
#include <renderer.h>

static void push_position(char* dst, int x, int y, uint32_t tile_width, uint32_t tile_height)
{
    *((float*)(dst + 24 * 0 + 0)) = (x + 0) * tile_width;
    *((float*)(dst + 24 * 0 + 4)) = (y + 0) * tile_height;
    *((float*)(dst + 24 * 1 + 0)) = (x + 1) * tile_width;
    *((float*)(dst + 24 * 1 + 4)) = (y + 0) * tile_height;
    *((float*)(dst + 24 * 2 + 0)) = (x + 1) * tile_width;
    *((float*)(dst + 24 * 2 + 4)) = (y + 1) * tile_height;
    *((float*)(dst + 24 * 3 + 0)) = (x + 0) * tile_width;
    *((float*)(dst + 24 * 3 + 4)) = (y + 1) * tile_height;
}

static void push_texture(char* dst, uint16_t i)
{
    static const int tile_count = 32;
    static const float size = 1.f / tile_count;

    int tx;
    int ty;

    tx = i % 32;
    ty = i / 32;

    *((float*)(dst + 24 * 0 + 0)) = (tx + 0.f) * size;
    *((float*)(dst + 24 * 0 + 4)) = (ty + 0.f) * size;
    *((float*)(dst + 24 * 1 + 0)) = (tx + 1.f) * size;
    *((float*)(dst + 24 * 1 + 4)) = (ty + 0.f) * size;
    *((float*)(dst + 24 * 2 + 0)) = (tx + 1.f) * size;
    *((float*)(dst + 24 * 2 + 4)) = (ty + 1.f) * size;
    *((float*)(dst + 24 * 3 + 0)) = (tx + 0.f) * size;
    *((float*)(dst + 24 * 3 + 4)) = (ty + 1.f) * size;
}

static void push_color(char* dst, Color src)
{
    for (int i = 0; i < 4; ++i)
    {
        *((uint8_t*)(dst + 24 * i + 0)) = src.r;
        *((uint8_t*)(dst + 24 * i + 1)) = src.g;
        *((uint8_t*)(dst + 24 * i + 2)) = src.b;
    }
}

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

    _buffer_vertex.resize(size * 96);

    clear();
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
    size_t len;

    len = strlen(str);
    for (size_t i = 0; i < len; ++i)
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
    uint16_t    symbol;
    Color       color;
    Color       color_bg;
    size_t      idx;
    size_t      index_count;
    size_t      count;
    char*       data;

    _buffer_index.clear();

    data = &(_buffer_vertex[0]);
    index_count = 0;
    count = 0;

    for (size_t j = 0; j < _tiles_y; ++j)
    {
        for (size_t i = 0; i < _tiles_x; ++i)
        {
            idx = i + j * _tiles_x;
            symbol = _symbols[idx];
            if (symbol == 0)
                continue;
           color = _colors[idx];
           color_bg = _colors_bg[idx];

           push_position(data + count * 96, i, j, _tile_width, _tile_height);
           push_texture(data + count * 96 + 8, symbol);
           push_color(data + count * 96 + 16, color);
           push_color(data + count * 96 + 20, color_bg);
           _buffer_index.push_back(index_count);
           _buffer_index.push_back(index_count + 1);
           _buffer_index.push_back(index_count + 2);
           _buffer_index.push_back(index_count);
           _buffer_index.push_back(index_count + 2);
           _buffer_index.push_back(index_count + 3);
           index_count += 4;
           count += 1;
        }
    }

    glBindTexture(GL_TEXTURE_2D, _texture);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

    glBufferData(GL_ARRAY_BUFFER, count * 96, nullptr, GL_STREAM_DRAW);
    glBufferData(GL_ARRAY_BUFFER, count * 96, data, GL_STREAM_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _buffer_index.size() * 4, nullptr, GL_STREAM_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _buffer_index.size() * 4, _buffer_index.data(), GL_STREAM_DRAW);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glVertexPointer(2, GL_FLOAT, 24, (GLvoid*)0);
    glColorPointer(3, GL_UNSIGNED_BYTE, 24, (GLvoid*)20);
    glDrawElements(GL_TRIANGLES, _buffer_index.size(), GL_UNSIGNED_INT, (GLvoid*)0);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 24, (GLvoid*)8);
    glColorPointer(3, GL_UNSIGNED_BYTE, 24, (GLvoid*)16);
    glDrawElements(GL_TRIANGLES, _buffer_index.size(), GL_UNSIGNED_INT, (GLvoid*)0);
}

int Renderer::index(int x, int y) const
{
    if (x < 0 || x >= (int)_tiles_x || y < 0 || y >= (int)_tiles_y)
        return -1;
    return x + y * _tiles_x;
}
