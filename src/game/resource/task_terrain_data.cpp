#include <game/resource/task_terrain_data.h>
#include <serialize.h>

SparseArray<TaskTerrainData> TaskTerrainData::_data;

static void load_task(TaskTerrainData& task, MemoryFile& file)
{
    uint16_t len;

    file.read(&len);
    task.name = new char[len + 1];
    file.read(task.name, len);
    task.name[len] = 0;

    file.read(&len);
    task.match.resize(len);
    for (uint16_t i = 0; i < len; ++i)
    {
        uint16_t tile;
        file.read(&tile);
        task.match[i] = tile;
    }
    file.read(&task.into);
}

void TaskTerrainData::load(Archive& archive)
{
    unserialize_resource_array(TaskTerrainData::_data, archive, "task_terrain.bin", load_task);
}
