#include <cstdlib>
#include <cstring>
#include <SDL2/SDL.h>
#include <archive.h>
#include <image.h>
#include <log.h>
#include <util/file_path.h>

char* load_image(const char* name, uint32_t& width, uint32_t& height)
{
    Archive archive;
    SDL_Surface* img;
    SDL_Surface* img_rgba;
    char* pixels;
    char* file_buffer;
    uint32_t file_size;
    Uint32 rmask, gmask, bmask, amask;
    SDL_RWops* rw;

#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    archive.open(data_path("supremacy.bin"));
    archive.read(&file_buffer, &file_size, "tileset.bmp");
    archive.close();

    rw = SDL_RWFromMem(file_buffer, file_size);
    img = SDL_LoadBMP_RW(rw, 1);

    delete [] file_buffer;

    if (!img)
    {
        log_line(LogLevel::Fatal, "Could not load image: '%s'", name);
        exit(1);
    }
    width = img->w;
    height = img->h;
    img_rgba = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
    SDL_SetColorKey(img, SDL_TRUE, SDL_MapRGB(img->format, 0xff, 0x00, 0xff));
    SDL_BlitSurface(img, nullptr, img_rgba, nullptr);
    SDL_FreeSurface(img);

    size_t size = width * height * 4;
    pixels = new char[size];
    std::memcpy(pixels, img_rgba->pixels, size);
    SDL_FreeSurface(img_rgba);

    return pixels;
}
