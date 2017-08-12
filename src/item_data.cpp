#include <item_data.h>
#include <serialize.h>

SparseArray<ItemData> ItemData::_data;

static void load_item(ItemData& item, MemoryFile& file)
{
    file.read(&item.sym);
    file.read(&item.color.r);
    file.read(&item.color.g);
    file.read(&item.color.b);
}

void ItemData::load(Archive& archive)
{
    unserialize_resource_array(ItemData::_data, archive, "item.bin", load_item);
}
