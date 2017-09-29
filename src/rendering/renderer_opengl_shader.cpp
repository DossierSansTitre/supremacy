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

    log_line(LogLevel::Info, "Using the OpenGL Shader Renderer");
    archive.open(data_path("supremacy.bin"));
    shader_vert = load_shader(archive, GL_VERTEX_SHADER, "shaders/opengl/3.2/shader.vert.glsl");
    shader_frag = load_shader(archive, GL_FRAGMENT_SHADER, "shaders/opengl/3.2/shader.frag.glsl");
    _program = link_shader(shader_vert, shader_frag);
    init_buffers();
}

RendererOpenGLShader::~RendererOpenGLShader()
{

}

void RendererOpenGLShader::clear()
{

}

void RendererOpenGLShader::render()
{
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(_vao);
    glUseProgram(_program);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
}

void RendererOpenGLShader::init_buffers()
{
    static const uint16_t ibo_data[] = {
        0, 1, 2,
        0, 2, 3
    };

    static const float vbo_data[] = {
        -1.f, -1.f, 0.f, 0.f,
         1.f, -1.f, 1.f, 0.f,
         1.f,  1.f, 1.f, 1.f,
        -1.f,  1.f, 0.f, 1.f,
    };

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ibo);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 6, ibo_data, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, vbo_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}
