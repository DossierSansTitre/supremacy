#include <cmath>
#include <noise.h>
#include <map.h>
#include <biome.h>
#include <tile.h>
#include <math/algorithm.h>

static bool exposed_block(const Map& map, uint16_t tile, int x, int y, int z)
{
    return (map.tile_at(x, y, z) == TileID(tile) && map.tile_at(x, y, z + 1) == TileID(0));
}

static bool smooth_pass(Map& map, uint16_t from, uint16_t to, int x, int y, int z)
{
    if (!exposed_block(map, from, x, y, z - 1))
        return false;
    if (!exposed_block(map, from, x + 1, y, z)
        && !exposed_block(map, from, x - 1, y, z)
        && !exposed_block(map, from, x, y + 1, z)
        && !exposed_block(map, from, x, y - 1, z))
        return false;
    if (!exposed_block(map, from, x + 1, y, z - 1)
        && !exposed_block(map, from, x - 1, y, z - 1)
        && !exposed_block(map, from, x, y + 1, z - 1)
        && !exposed_block(map, from, x, y - 1, z - 1))
        return false;
    MaterialID mat = map.material_at(x, y, z - 1);
    map.set_material(x, y, z, mat);
    map.set_floor({x, y, z}, mat);
    map.set_tile(x, y, z, TileID(to));
    return true;
}

static void smooth_pass(Map& map, Array<int>& height_map, uint16_t from, uint16_t to)
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
                if (smooth_pass(map, from, to, i, j, k))
                    height_map[j * map.width() + i]++;
            }
        }
    }
}

static void fill_height(Map& map, int x, int y, int base, int length, TileID tile, MaterialID material)
{
    if (base == 0 && length)
        map.set_floor({x, y, 0}, material);
    for (int z = 0; z < length; ++z)
    {
        map.set_tile(x, y, base + z, tile);
        map.set_material(x, y, base + z, material);
        map.set_floor({x, y, base + z + 1}, material);
    }
}

void generate_map(Map& map, BiomeID biome_id, uint32_t seed)
{
    static const int width = 512;
    static const int height = 512;
    static const int depth = 64;

    Array<int> height_map;
    height_map.resize(width * height);

    const Biome& biome = Biome::from_id(biome_id);
    map.create(width, height, depth);

    iterate(Vector2i(height, width), [&] (Vector2i pos) {
        float f = perlin_octave_corrected(seed, pos.x, pos.y, 2.f, 7, 2.f);
        float variable_part = biome.noise_amplitude / 100.f;

        f = std::pow(f, biome.noise_power);
        int h = (30 * (1.f - variable_part)) + 30 * variable_part * f;
        fill_height(map, pos.x, pos.y, 0, h, 0x01, biome.layers[2]);
        fill_height(map, pos.x, pos.y, h, 8, 0x01, biome.layers[1]);
        fill_height(map, pos.x, pos.y, h + 8, 1, 0x01, biome.layers[0]);
        height_map[pos.x + width * pos.y] = h + 9;
    });
    smooth_pass(map, height_map, 0x01, 0x03);
    iterate(Vector2i(height, width), [&] (Vector2i pos) {
        int h = height_map[pos.x + pos.y * width];
        if (rand() % 10000 < biome.tree_density)
        {
            map.set_tile(pos.x, pos.y, h, biome.tree_tile);
            map.set_material(pos.x, pos.y, h, biome.tree_material);
        }
    });
    map.compute_visibility();
}
