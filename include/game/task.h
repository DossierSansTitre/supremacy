#ifndef TASK_H
#define TASK_H

#include <types.h>

enum class TaskType
{
    None,
    Terrain
};

struct TaskTerrain
{
    TaskTerrainID   id;
    Vector3i        target;
};

struct Task
{
    TaskType type;
    union
    {
        TaskTerrain terrain;
    };
};

#endif
