#include <worldmap_generator.h>
#include <noise.h>
#include <biome.h>
#include <log.h>
#include <math/algorithm.h>

static float mask_border(int value, int length)
{
    static const int border = 5;

    if (value < border)
        return float(value) / border;
    else if (value >= length - border - 1)
        return float(length - value - 1) / border;
    return 1.f;
}

static void generate_heightmap(Array<u16>& heightmap, Vector2i size, u32 seed)
{
    float value;
    float mask;

    iterate(size, [&] (Vector2i v) {
        value = noise_fractal_octave_2d(seed, v.x, v.y, 2.f, 7);
        mask = mask_border(v.x, size.x);
        mask *= mask_border(v.y, size.y);
        heightmap[v.x + v.y * size.x] = value * mask * 1000;
    });
}

static void generate_value_map(Array<u16>& value_map, Vector2i size, u16 max, u32 seed)
{
    float value;

    iterate(size, [&] (Vector2i v) {
        value = noise_fractal_octave_2d(seed, v.x, v.y, 2.f, 7);
        value_map[v.x + v.y * size.x] = value * max;
    });
}

WorldmapGenerator::WorldmapGenerator()
{

}

WorldmapGenerator::~WorldmapGenerator()
{

}


Worldmap* WorldmapGenerator::generate(u16 id, Vector2i size, Rng& rng)
{
    Worldmap*   worldmap;
    u16         height;
    u16         temperature;
    BiomeID     biome_id;
    Rng         world_rng;
    size_t      index;

    world_rng.seed(rng);
    worldmap = new Worldmap(id, size);

    generate_heightmap(worldmap->_height, size, world_rng.rand());
    generate_value_map(worldmap->_temperature, size, 1000, world_rng.rand());
    generate_value_map(worldmap->_rain, size, 1000, world_rng.rand());
    generate_value_map(worldmap->_drainage, size, 1000, world_rng.rand());
    iterate(size, [&] (auto v) {
        biome_id = 0;
        index = v.x + v.y * size.x;
        height = worldmap->_height[index];
        temperature = worldmap->_temperature[index];
        for (size_t i = 1; i < Biome::count(); ++i)
        {
            const auto& b = Biome::from_id(i);

            if (b.height_max == 0)
                continue;
            if (height < b.height_min || height >= b.height_max)
                continue;
            if (temperature < b.temperature_min || temperature >= b.temperature_max)
                continue;
            biome_id = i;
            break;
        }
        if (!biome_id)
            log_line(LogLevel::Fatal, "Could not find suitable biome (Height: %d, Temp: %d)", height, temperature);
        worldmap->_biomes[index] = biome_id;
    });
    return worldmap;
}
