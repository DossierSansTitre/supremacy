#include <noise.h>
#include <map.h>

static void fill_height(Map& map, int x, int y, int base, int length, TileID tile, MaterialID material)
{
    for (int z = 0; z < length; ++z)
    {
        map.set_tile(x, y, base + z, tile);
        map.set_material(x, y, base + z, material);
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
            fill_height(map, i, j, 0, h - 4, TileID::Block, MaterialID::Rock);
            fill_height(map, i, j, h - 4, 4, TileID::Block, MaterialID::Dirt);
        }
    }
    map.compute_visibility();
}
