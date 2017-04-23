#include <opengl.h>
#include <image.h>

GLuint load_texture_data(const char* data, uint32_t width, uint32_t height)
{
    GLuint tex;

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return tex;
}

GLuint load_texture(const char* name, uint32_t& width, uint32_t& height)
{
    GLuint tex;
    char* data;

    data = load_image(name, width, height);
    tex = load_texture_data(data, width, height);
    delete [] data;

    return tex;
}
