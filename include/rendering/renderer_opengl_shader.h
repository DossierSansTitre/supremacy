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
    GLuint  _program;
};

#endif
