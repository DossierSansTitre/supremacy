#ifndef ITEM_DATA_H
#define ITEM_DATA_H

#include <types.h>
#include <std/sparse_array.h>
#include <color.h>
#include <archive.h>

struct ItemData
{
    uint16_t    sym;
    Color       color;

    static SparseArray<ItemData> _data;

    static const ItemData&  from_id(ItemID id) { return _data[id]; }
    static void load(Archive& archive);
};

#endif
