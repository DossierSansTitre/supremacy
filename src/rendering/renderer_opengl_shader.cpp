#include <cstdlib>
#include <cstring>
#include <rendering/renderer_opengl_shader.h>
#include <opengl.h>
#include <log.h>
#include <archive.h>
#include <util/file_path.h>

static GLuint load_shader(Archive& archive, GLenum type, const char* path)
{
    GLuint shader;
    GLint compile_status;
    uint32_t shader_data_size;
    char* shader_data;
    char* shader_text;

    shader = glCreateShader(type);
    archive.read(&shader_data, &shader_data_size, path);
    shader_text = new char[shader_data_size + 1];
    std::memcpy(shader_text, shader_data, shader_data_size);
    shader_text[shader_data_size] = 0;
    delete [] shader_data;
    glShaderSource(shader, 1, &shader_text, nullptr);
    delete [] shader_text;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE)
    {
        GLint log_size;
        char* log_message;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
        log_message = new char[log_size + 1];
        log_message[log_size] = 0;
        glGetShaderInfoLog(shader, log_size, &log_size, log_message);
        log_line(LogLevel::Fatal, "Shader Compile Error: %s", log_message);
        exit(1);
    }
    return shader;
}

static GLuint link_shader(GLuint vert, GLuint frag)
{
    GLuint program;
    GLint link_status;

    program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glBindAttribLocation(program, 0, "vPosition");
    glBindAttribLocation(program, 1, "vTexCoord");
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE)
    {
        GLint log_size;
        char* log_message;

        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);
        log_message = new char[log_size + 1];
        log_message[log_size] = 0;
        glGetProgramInfoLog(program, log_size, &log_size, log_message);
        log_line(LogLevel::Fatal, "Shader Link Error: %s", log_message);
        exit(1);
    }
    return program;
}

RendererOpenGLShader::RendererOpenGLShader(Window& window, DrawBuffer& draw_buffer)
: Renderer(window, draw_buffer)
{
    Archive archive;
    GLuint shader_vert;
    GLuint shader_frag;

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    log_line(LogLevel::Info, "Using the OpenGL Shader Renderer");
    archive.open(data_path("supremacy.bin"));
    shader_vert = load_shader(archive, GL_VERTEX_SHADER, "shaders/opengl/3.2/shader.vert.glsl");
    shader_frag = load_shader(archive, GL_FRAGMENT_SHADER, "shaders/opengl/3.2/shader.frag.glsl");
    _program = link_shader(shader_vert, shader_frag);
    _texture_uniform = glGetUniformLocation(_program, "uTexture");
    _symbol_uniform = glGetUniformLocation(_program, "uSymbol");
    _color_uniform = glGetUniformLocation(_program, "uColor");
    _color_bg_uniform = glGetUniformLocation(_program, "uColorBg");
    _tile_count_uniform = glGetUniformLocation(_program, "uTileCount");
    init_buffers();
    init_textures();
    resize({(_window.width() / _texture_size.x) & (~1), _window.height() / _texture_size.y});
    glBindVertexArray(_vao);
    glUseProgram(_program);
    glUniform1i(_texture_uniform, 0);
    glUniform1i(_symbol_uniform, 1);
    glUniform1i(_color_uniform, 2);
    glUniform1i(_color_bg_uniform, 3);
    glUniform2f(_tile_count_uniform, _draw_buffer.width(), _draw_buffer.height());
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, _symbol);
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, _color);
    glActiveTexture(GL_TEXTURE0 + 3);
    glBindTexture(GL_TEXTURE_2D, _color_bg);
}

RendererOpenGLShader::~RendererOpenGLShader()
{

}

void RendererOpenGLShader::clear()
{
    _draw_buffer.resize((_window.width() / _texture_size.x) & (~1), _window.height() / _texture_size.y);
}

void RendererOpenGLShader::render()
{
    auto& db = _draw_buffer;

    size_t w = db.width();
    size_t h = db.height();

    glBindTexture(GL_TEXTURE_2D, _symbol);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RED_INTEGER, GL_UNSIGNED_SHORT, db.symbol());
    glBindTexture(GL_TEXTURE_2D, _color);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, db.color());
    glBindTexture(GL_TEXTURE_2D, _color_bg);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, db.color_bg());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);
}


void RendererOpenGLShader::resize(Vector2u size)
{
    auto& db = _draw_buffer;

    db.resize(size.x, size.y);
    size_t w = db.width();
    size_t h = db.height();

    glBindTexture(GL_TEXTURE_2D, _symbol);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16UI, w, h, 0, GL_RED_INTEGER, GL_UNSIGNED_SHORT, nullptr);
    glBindTexture(GL_TEXTURE_2D, _color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, _color_bg);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

void RendererOpenGLShader::init_buffers()
{
    static const uint8_t ibo_data[] = {
        0, 1, 2,
        0, 2, 3
    };

    static const float vbo_data[] = {
        -1.f,  1.f, 0.f, 0.f,
         1.f,  1.f, 1.f, 0.f,
         1.f, -1.f, 1.f, 1.f,
        -1.f, -1.f, 0.f, 1.f,
    };

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ibo);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint8_t) * 6, ibo_data, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, vbo_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

static GLuint create_texture()
{
    GLuint tex;

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return tex;
}

void RendererOpenGLShader::init_textures()
{
    uint32_t w;
    uint32_t h;

    _texture = load_texture("tileset.png", &w, &h);
    _texture_size.x = w / 32;
    _texture_size.y = h / 32;
    _symbol = create_texture();
    _color = create_texture();
    _color_bg = create_texture();
}
