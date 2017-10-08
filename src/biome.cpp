#include <biome.h>
#include <serialize.h>

SparseArray<Biome> Biome::_data;

static void load_biome(Biome& biome, MemoryFile& file)
{
    u16 name_len;
    u16 flags;

    file.read(&name_len);
    biome.name = new char[name_len + 1];
    biome.name[name_len] = 0;
    file.read(biome.name, name_len);
    file.read(&biome.color.r);
    file.read(&biome.color.g);
    file.read(&biome.color.b);
    file.read(&biome.color_bg.r);
    file.read(&biome.color_bg.g);
    file.read(&biome.color_bg.b);
    file.read(&biome.symbol);
    file.read(&biome.height_min);
    file.read(&biome.height_max);
    file.read(&biome.temperature_min);
    file.read(&biome.temperature_max);
    file.read(&biome.rain_min);
    file.read(&biome.rain_max);
    file.read(&biome.drainage_min);
    file.read(&biome.drainage_max);

    file.read(&biome.layers);
    file.read(&biome.noise_amplitude);
    file.read(&biome.noise_power);

    file.read(&biome.tree_density);
    file.read(&biome.tree_tile);
    file.read(&biome.tree_material);

    file.read(&flags);
    biome.no_embark = flags & 0x01;
}

void Biome::load(Archive& archive)
{
    unserialize_resource_array(Biome::_data, archive, "biome.bin", load_biome);
}
