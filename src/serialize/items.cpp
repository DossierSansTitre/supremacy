#include <serialize.h>
#include <items.h>

void serialize_items(std::ofstream& stream, const Items& items)
{
    u32 size;
    u32 free_size;

    size = items._item_id.size();
    free_size = items._free.size();

    stream.write((char*)&size, sizeof(size));
    stream.write((char*)&free_size, sizeof(free_size));

    serialize(stream, items._item_id.data(), size);
    serialize(stream, items._pos.data(), size);
    serialize(stream, items._item_count.data(), size);
    serialize(stream, items._free.data(), free_size);
    serialize(stream, &items._count, 1);
}

void unserialize_items(Items& items, std::ifstream& stream)
{
    u32 size;
    u32 free_size;

    stream.read((char*)&size, sizeof(size));
    stream.read((char*)&free_size, sizeof(free_size));

    items._item_id.resize(size);
    items._pos.resize(size);
    items._item_count.resize(size);
    items._free.resize(free_size);

    unserialize(stream, items._item_id.data(), size);
    unserialize(stream, items._pos.data(), size);
    unserialize(stream, items._item_count.data(), size);
    unserialize(stream, items._free.data(), free_size);
    unserialize(stream, &items._count, 1);
}
