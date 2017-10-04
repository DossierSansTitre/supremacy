#include <opengl.h>
#include <archive.h>
#include <memory_file.h>
#include <util/file_path.h>
#include <util/png.h>

// TODO: Refactor this file

GLuint load_texture_data(const char* data, uint32_t width, uint32_t height)
{
    GLuint tex;

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return tex;
}

GLuint load_texture(const char* name, uint32_t* width, uint32_t* height)
{
    Archive archive;
    MemoryFile file;
    GLuint tex;
    char* raw_file;
    uint32_t raw_file_size;
    char* data;

    archive.open(data_path("/supremacy.bin"));
    archive.read(&raw_file, &raw_file_size, name);
    file.open(raw_file, raw_file_size);
    data = load_png(file, width, height);
    tex = load_texture_data(data, *width, *height);
    delete [] data;
    delete [] raw_file;

    return tex;
}
