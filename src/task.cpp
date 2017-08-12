#include <task.h>
#include <serialize.h>

SparseArray<Task> Task::_data;

static void load_task(Task& task, MemoryFile& file)
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

void Task::load(Archive& archive)
{
    unserialize_resource_array(Task::_data, archive, "task.bin", load_task);
}
