#include <material.h>
#include <serialize.h>

Array<Material> Material::data;

static void load_material(Material& mat, MemoryFile& file)
{
    uint16_t drop_item;
    uint8_t flags;

    file.read(&mat.color.r);
    file.read(&mat.color.g);
    file.read(&mat.color.b);

    file.read(&mat.color_bg.r);
    file.read(&mat.color_bg.g);
    file.read(&mat.color_bg.b);

    file.read(&drop_item);
    mat.dropping_item = static_cast<ItemID>(drop_item);

    file.read(&flags);
    mat.minable = flags & 1;
}

void Material::load(Archive& archive)
{
    unserialize_resource_array(Material::data, archive, "material.bin", load_material);
}
