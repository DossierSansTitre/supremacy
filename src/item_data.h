#ifndef ITEM_DATA_H
#define ITEM_DATA_H

#include <cstdint>
#include <item_id.h>
#include <color.h>

struct ItemData
{
    uint16_t    sym;
    Color       color;

    static const ItemData   item_data[];

    static const ItemData&  from_id(ItemID id) { return item_data[static_cast<int>(id)]; }
};

#endif
