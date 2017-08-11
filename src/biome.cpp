#include <biome.h>
#include <serialize.h>

Array<Biome> Biome::data;

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
    uint16_t layer_count;
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
}

void Biome::load(Archive& archive)
{
    unserialize_resource_array(Biome::data, archive, "biome.bin", load_biome);
}
