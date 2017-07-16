#include <material.h>
#include <item_id.h>

Array<Material> Material::data;

void Material::load(Archive& archive)
{
    SupFile sup;
    MemoryFile file;

    sup.open(archive, "material.bin");
    while (sup.read(file))
    {
        uint16_t id;
        uint16_t drop_item;
        uint8_t flags;

        file.read(&id);
        data.resize(id + 1);
        Material& mat = data.back();

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
}
