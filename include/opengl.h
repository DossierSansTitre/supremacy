#ifndef OPENGL_H
#define OPENGL_H

#include <SDL2/SDL.h>

#if defined(__APPLE__)
# include <OpenGL/gl.h>
#else
# include <GL/glew.h>
#endif

GLuint  load_texture_data(const char* data, uint32_t width, uint32_t height);
GLuint  load_texture(const char* name, uint32_t& width, uint32_t& height);

#endif