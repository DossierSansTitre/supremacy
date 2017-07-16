#include <item_data.h>

Array<ItemData> ItemData::data;

void ItemData::load(Archive& archive)
{
    SupFile sup;
    MemoryFile file;

    sup.open(archive, "item.bin");
    while (sup.read(file))
    {
        uint16_t id;

        file.read(&id);
        data.resize(id + 1);
        ItemData& item = data.back();
        file.read(&item.sym);
        file.read(&item.color.r);
        file.read(&item.color.g);
        file.read(&item.color.b);
    }
}
