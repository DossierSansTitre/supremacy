#ifndef GAME_RESOURCE_TASK_TERRAIN_DATA_H
#define GAME_RESOURCE_TASK_TERRAIN_DATA_H

#include <std/sparse_array.h>
#include <archive.h>

struct TaskTerrainData
{
    char*               name;
    Array<uint16_t>     match;
    uint16_t            into;

    static SparseArray<TaskTerrainData> _data;

    static const TaskTerrainData& from_id(uint16_t id) { return _data[id]; }
    static void load(Archive& archive);
};

#endif
