#include <noise.h>
#include <map.h>

static bool exposed_block_check(const Map& map, int x, int y, int z)
{
    return (map.tile_at(x, y, z) == TileID::Block && map.tile_at(x, y, z + 1) == TileID::None);
}

static bool ramp_target_check(const Map& map, int x, int y, int z)
{
    return (map.tile_at(x, y, z) == TileID::Block && (map.tile_at(x, y, z + 1) == TileID::None || map.tile_at(x, y, z + 1) == TileID::Floor));
}

static void ramp_pass(Map& map, int x, int y, int z)
{
    if (!exposed_block_check(map, x, y, z - 1))
        return;
    map.set_material(x, y, z, map.material_at(x, y, z - 1));
    if (ramp_target_check(map, x + 1, y, z)
        || ramp_target_check(map, x - 1, y, z)
        || ramp_target_check(map, x, y + 1, z)
        || ramp_target_check(map, x, y - 1, z))
    {
        map.set_tile(x, y, z, TileID::Ramp);
    }
    else
    {
        if (rand() % 5 == 0) {
            map.set_material(x, y, z, MaterialID::Wood);
            map.set_tile(x, y, z, TileID::Tree);
        }
        else
            map.set_tile(x, y, z, TileID::Floor);
    }
}

static void ramp_pass(Map& map)
{
    int width;
    int height;
    int depth;

    width = map.width();
    height = map.height();
    depth = map.depth();

    for (int k = 0; k < depth; ++k)
    {
        for (int j = 0; j < height; ++j)
        {
            for (int i = 0; i < width; ++i)
            {
                ramp_pass(map, i, j, k);
            }
        }
    }
}

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
            h = static_cast<int>(15 + n * 10);
            fill_height(map, i, j, 0, h, TileID::Block, MaterialID::Rock);
            fill_height(map, i, j, h, 10, TileID::Block, MaterialID::Dirt);
            fill_height(map, i, j, h + 10, 1, TileID::Block, MaterialID::Grass);
        }
    }
    ramp_pass(map);
    map.compute_visibility();
}
