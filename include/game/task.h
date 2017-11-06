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
    Vector3i    target;
};

#endif
