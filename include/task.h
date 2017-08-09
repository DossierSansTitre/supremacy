#ifndef TASK_H
#define TASK_H

#include <std/array.h>
#include <archive.h>

struct Task
{
    char*               name;
    Array<uint16_t>     match;
    uint16_t            into;

    static Array<Task> _data;

    static const Task& from_id(uint16_t id) { return _data[id]; }
    static void load(Archive& archive);
};

#endif
