#ifndef RENDERER_OPENGL_SHADER_H
#define RENDERER_OPENGL_SHADER_H

#include <rendering/renderer.h>

class RendererOpenGLShader : public Renderer
{
public:
    RendererOpenGLShader(Window& window, DrawBuffer& draw_buffer);
    virtual ~RendererOpenGLShader();

    virtual void clear() override;
    virtual void render() override;

private:
    void    init_buffers();

    GLuint  _program;
    GLuint  _vao;
    GLuint  _vbo;
    GLuint  _ibo;
    GLuint  _texture;
    GLuint  _symbol;
    GLuint  _color;
    GLuint  _color_bg;
};

#endif
