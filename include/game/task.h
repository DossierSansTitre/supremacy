#ifndef TASK_H
#define TASK_H

#include <types.h>

enum class TaskType
{
    None,
    Terrain
};

struct Task
{
    TaskType    type;
    uint16_t    terrain_task_id;
    Vector3i    target;
    Path        path;
};

#endif
