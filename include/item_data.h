#ifndef ITEM_DATA_H
#define ITEM_DATA_H

#include <types.h>
#include <std/array.h>
#include <color.h>
#include <archive.h>

struct ItemData
{
    uint16_t    sym;
    Color       color;

    static Array<ItemData> data;

    static const ItemData&  from_id(ItemID id) { return data[static_cast<int>(id)]; }
    static void load(Archive& archive);
};

#endif
