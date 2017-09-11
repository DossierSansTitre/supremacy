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

static void generate_value_map(Array<u16>& value_map, Vector2i size, u16 max, u32 seed)
{
    float value;

    iterate(size, [&] (Vector2i v) {
        value = noise_fractal_octave_2d(seed, v.x, v.y, 1.6f, 7);
        value *= 1.5f;
        value = (value + 1.f) / 2.f;
        if (value < 0.f)
            value = 0.f;
        else if (value > 1.f)
            value = 1.f;
        value_map[v.x + v.y * size.x] = value * (max - 1);
    });
}

static void generate_heightmap(Array<u16>& heightmap, Vector2i size, u32 seed)
{
    Vector2i center;
    Vector2i delta;

    generate_value_map(heightmap, size, 1000, seed);
    center = size / 2;
    i32 value;
    float inv_size = 1.f / size.x;
    float inv_square = inv_size * inv_size;

    iterate(size, [&] (Vector2i v) {
        size_t i = v.x + size.x * v.y;
        delta = center - v;

        value = heightmap[i];
        value -= (((delta.x * delta.x + delta.y * delta.y)) * (4096.f / (size.x * size.x)) - 175);
        if (value > 1000)
            value = 1000;
        else if (value < 0)
            value = 0;
        heightmap[i] = value;
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
    u16         rain;
    u16         drainage;

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
        index = worldmap->index(v);
        height = worldmap->_height[index];
        temperature = worldmap->_temperature[index];
        rain = worldmap->_rain[index];
        drainage = worldmap->_drainage[index];
        for (size_t i = 1; i < Biome::count(); ++i)
        {
            const auto& b = Biome::from_id(i);

            if (b.height_max == 0)
                continue;
            if (height < b.height_min || height >= b.height_max)
                continue;
            if (temperature < b.temperature_min || temperature >= b.temperature_max)
                continue;
            if (rain < b.rain_min || rain >= b.rain_max)
                continue;
            if (drainage < b.drainage_min || drainage >= b.drainage_max)
                continue;
            biome_id = i;
            break;
        }
        if (!biome_id)
            log_line(LogLevel::Fatal, "Could not find suitable biome (Height: %d, Temp: %d, Rain: %d, Drain: %d)", height, temperature, rain, drainage);
        worldmap->_biomes[index] = biome_id;
    });
    return worldmap;
}
