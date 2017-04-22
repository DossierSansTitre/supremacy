#ifndef ITEM_DATA_H
#define ITEM_DATA_H

#include <item_id.h>

struct ItemData
{
    int     sym;
    int     color;
    int     bg_color;

    static const ItemData   item_data[];

    static const ItemData&  from_id(ItemID id) { return item_data[static_cast<int>(id)]; }
};

#endif
