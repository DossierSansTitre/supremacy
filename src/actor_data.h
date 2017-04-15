#ifndef ACTOR_DATA_H
#define ACTOR_DATA_H

#include <actor_id.h>

struct ActorData
{
    int     sym;
    int     color;
    int     bgcolor;
    int     max_health;
    int     max_speed;

    static const ActorData actor_data[];

    static const ActorData& from_id(ActorID id) { return actor_data[static_cast<int>(id)]; }
};

#endif
