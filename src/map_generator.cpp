#include <cmath>
#include <noise.h>
#include <map.h>
#include <biome.h>
#include <tile.h>

#include <iostream>

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
    map.set_material(x, y, z, map.material_at(x, y, z - 1));
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

static void replace_pass(Map& map, uint16_t from_tile, uint16_t from_material, uint16_t to_tile, uint16_t to_material, uint32_t frequency)
{
    for (int k = 0; k < map.depth(); ++k)
    {
        for (int j = 0; j < map.height(); ++j)
        {
            for (int i = 0; i < map.width(); ++i)
            {
                if (map.tile_at(i, j, k) == TileID(from_tile)  && map.material_at(i, j, k) == MaterialID(from_material))
                {
                    if (rand() % frequency == 0)
                    {
                        map.set_tile(i, j, k, TileID(to_tile));
                        map.set_material(i, j, k, MaterialID(to_material));
                    }
                }
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

void generate_map(Map& map, uint32_t seed)
{
    static const int width = 512;
    static const int height = 512;
    static const int depth = 64;

    Array<int> height_map;
    height_map.resize(width * height);
    for (int i = 0; i < width * height; ++i)
        height_map[i] = 0;

    const Biome& biome = Biome::from_id(0x01);

    map.create(width, height, depth);

    for (const auto& layer : biome.layers)
    {
        switch (layer.type)
        {
            case Biome::LayerType::Fill:
                for (int j = 0; j < height; ++j)
                {
                    for (int i = 0; i < width; ++i)
                    {
                        int base;
                        base = height_map[j * width + i];
                        if (base == 0 || Tile::from_id(map.tile_at(i, j, base - 1)).block)
                        {
                            fill_height(map, i, j, base, layer.fill.height, TileID(layer.fill.tile), MaterialID(layer.fill.material));
                            height_map[j * width + i] += layer.fill.height;
                        }
                    }
                }
                break;
            case Biome::LayerType::Octave:
                for (int j = 0; j < height; ++j)
                {
                    for (int i = 0; i < width; ++i)
                    {
                        float n;
                        n = noise_fractal_octave_2d(seed, i, j, layer.octave.persistance, layer.octave.octaves);
                        n = pow(n, layer.octave.power);

                        int h;
                        h = layer.octave.a * n + layer.octave.b;
                        fill_height(map, i, j, height_map[j * width + i], h, TileID(layer.octave.tile), MaterialID(layer.octave.material));
                        height_map[j * width + i] += h;
                    }
                }
                break;
            case Biome::LayerType::Smooth:
                smooth_pass(map, height_map, layer.smooth.from, layer.smooth.to);
                break;
            case Biome::LayerType::Replace:
                replace_pass(map, layer.replace.from_tile, layer.replace.from_material, layer.replace.to_tile, layer.replace.to_material, layer.replace.frequency);
                break;
            default:
                break;
        }
    }
    map.compute_visibility();
}
