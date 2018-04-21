#include <png.h>
#include <util/png.h>
#include <log.h>
#include <memory_file.h>

static void read_helper(png_structp png_ptr, png_bytep out_bytes, png_size_t byte_count)
{
    MemoryFile* file = (MemoryFile*)png_get_io_ptr(png_ptr);

    file->read(out_bytes, byte_count);
}

char* load_png(MemoryFile& file, uint32_t* out_width, uint32_t* out_height)
{
    png_byte header[8];
    char* data;

    file.read(header, 8);
    if (png_sig_cmp(header, 0, 8))
    {
        log_line(LogLevel::Error, "Bad PNG signature");
        return nullptr;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_set_read_fn(png_ptr, &file, &read_helper);
    png_set_sig_bytes(png_ptr, 8);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    png_read_info(png_ptr, info_ptr);
    png_uint_32 width = 0;
    png_uint_32 height = 0;
    int bit_depth = 0;
    int color_type = -1;
    png_get_IHDR(png_ptr, info_ptr,
        &width,
        &height,
        &bit_depth,
        &color_type,
        NULL, NULL, NULL);
    png_set_packing(png_ptr);
    png_set_palette_to_rgb(png_ptr);
    const png_size_t bytes_per_row = png_get_rowbytes(png_ptr, info_ptr);
    data = new char[width * height * 4];
    char* row_buffer = new char[bytes_per_row];
    for (size_t j = 0; j < height; ++j)
    {
        png_read_row(png_ptr, (png_bytep)row_buffer, NULL);
        for (size_t i = 0; i < width; ++i)
        {
            size_t index = i + j * width;
            data[index * 4 + 0] = row_buffer[i * 4 + 2];
            data[index * 4 + 1] = row_buffer[i * 4 + 1];
            data[index * 4 + 2] = row_buffer[i * 4 + 0];
            data[index * 4 + 3] = row_buffer[i * 4 + 3];
        }
    }
    delete [] row_buffer;
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    *out_width = width;
    *out_height = height;
    return data;
}
