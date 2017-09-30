#ifndef RENDERER_OPENGL_SHADER_H
#define RENDERER_OPENGL_SHADER_H

#include <rendering/renderer.h>
#include <math/vector.h>
#include <opengl.h>

class RendererOpenGLShader : public Renderer
{
public:
    RendererOpenGLShader(Window& window, DrawBuffer& draw_buffer);
    virtual ~RendererOpenGLShader();

    virtual void    clear() override;
    virtual void    render() override;

    void    resize(Vector2u size);

private:
    void    init_buffers();
    void    init_textures();

    GLuint  _program;
    GLuint  _vao;
    GLuint  _vbo;
    GLuint  _ibo;
    GLuint  _texture;
    GLuint  _symbol;
    GLuint  _color;
    GLuint  _color_bg;
    GLint   _texture_uniform;
    GLint   _symbol_uniform;
    GLint   _color_uniform;
    GLint   _color_bg_uniform;
    GLint   _tile_count_uniform;

    Vector2i _texture_size;
};

#endif
