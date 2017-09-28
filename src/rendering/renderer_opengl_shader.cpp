#include <rendering/renderer_opengl_shader.h>
#include <log.h>

RendererOpenGLShader::RendererOpenGLShader(Window& window, DrawBuffer& draw_buffer)
: Renderer(window, draw_buffer)
{
    log_line(LogLevel::Info, "Using the OpenGL Shader Renderer");
}

RendererOpenGLShader::~RendererOpenGLShader()
{

}

void RendererOpenGLShader::clear()
{

}

void RendererOpenGLShader::render()
{

}
