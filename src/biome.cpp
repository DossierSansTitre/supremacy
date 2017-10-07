#include <biome.h>
#include <serialize.h>

SparseArray<Biome> Biome::_data;

static void load_layer_fill(Biome::Layer& layer, MemoryFile& file)
{
    layer.type = Biome::LayerType::Fill;
    auto& fill = layer.fill;
    file.read(&fill.height);
    file.read(&fill.tile);
    file.read(&fill.material);
}

static void load_layer_octave(Biome::Layer& layer, MemoryFile& file)
{
    layer.type = Biome::LayerType::Octave;
    auto& octave = layer.octave;
    file.read(&octave.persistance);
    file.read(&octave.octaves);
    file.read(&octave.power);
    file.read(&octave.a);
    file.read(&octave.b);
    file.read(&octave.tile);
    file.read(&octave.material);
}

static void load_layer_smooth(Biome::Layer& layer, MemoryFile& file)
{
    layer.type = Biome::LayerType::Smooth;
    auto& smooth = layer.smooth;
    file.read(&smooth.from);
    file.read(&smooth.to);
}

static void load_layer_replace(Biome::Layer& layer, MemoryFile& file)
{
    layer.type = Biome::LayerType::Replace;
    auto& replace = layer.replace;
    file.read(&replace.from_tile);
    file.read(&replace.from_material);
    file.read(&replace.to_tile);
    file.read(&replace.to_material);
    file.read(&replace.frequency);
}

static void load_biome(Biome& biome, MemoryFile& file)
{
    u16 name_len;
    u16 layer_count;
    u8 flags;

    file.read(&name_len);
    biome.name = new char[name_len + 1];
    biome.name[name_len] = 0;
    file.read(biome.name, name_len);

    file.read(&layer_count);
    biome.layers.resize(layer_count);
    for (uint16_t i = 0; i < layer_count; ++i)
    {
        auto& layer = biome.layers[i];
        uint16_t type;
        file.read(&type);
        switch(type)
        {
            case 0x01:
                load_layer_fill(layer, file);
                break;
            case 0x02:
                load_layer_octave(layer, file);
                break;
            case 0x03:
                load_layer_smooth(layer, file);
                break;
            case 0x04:
                load_layer_replace(layer, file);
                break;
        }
    }
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
