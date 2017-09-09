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

static void generate_heightmap(Array<i16>& heightmap, Vector2i size, u32 seed)
{
    heightmap.resize(size.x * size.y);
    float value;
    float mask;

    iterate(size, [&] (Vector2i v) {
        value = noise_fractal_octave_2d(seed, v.x, v.y, 2.f, 7);
        mask = mask_border(v.x, size.x);
        mask *= mask_border(v.y, size.y);
        heightmap[v.x + v.y * size.x] = value * mask * 1500 - 500;
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
    Array<i16>  heightmap;
    Array<u16>  biomes;
    i16         height;
    Rng         world_rng;

    world_rng.seed(rng);
    worldmap = new Worldmap(id, size);

    generate_heightmap(heightmap, size, world_rng.rand());
    iterate(size, [&] (auto v) {
        biomes.clear();
        for (size_t i = 1; i < Biome::count(); ++i)
        {
            const auto& b = Biome::from_id(i);

            if (b.height_max == 0)
                continue;
            height = heightmap[v.x + v.y * size.x];
            if (height >= b.height_min && height < b.height_max)
                biomes.push_back(i);
        }
        worldmap->set_biome(v, biomes[rand() % biomes.size()]);
    });
    return worldmap;
}
