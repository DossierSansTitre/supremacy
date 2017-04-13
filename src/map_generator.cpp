#include <noise.h>
#include <map.h>

static void fill_height(Map& map, int x, int y, int height, TileID tile, MaterialID material)
{
    for (int z = 0; z < height; ++z)
    {
        map.set_tile(x, y, z, tile);
        map.set_material(x, y, z, material);
    }
}

void generate_map(Map& map, uint32_t seed)
{
    static const int width = 512;
    static const int height = 512;
    static const int depth = 64;

    float n;
    int h;

    map.create(width, height, depth);

    for (int j = 0; j < height; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            n = noise_fractal_octave_2d(seed, i, j, 2.0f, 6);
            n *= n;
            h = 15 + n * 10;
            fill_height(map, i, j, h, TileID::Block, MaterialID::Rock);
        }
    }
}
