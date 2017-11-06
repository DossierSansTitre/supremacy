#ifndef ACTORS_H
#define ACTORS_H

#include <fstream>
#include <actor_id.h>
#include <action_id.h>
#include <math/vector.h>
#include <path_finder.h>
#include <path.h>
#include <game/task.h>
#include <std/array.h>

class Actors
{
public:
    friend void serialize_actors(std::ofstream& stream, const Actors& actors);
    friend void unserialize_actors(Actors& actors, std::ifstream& stream);

    Actors();
    ~Actors();

    int         add(ActorID actor_id, Vector3i pos);
    void        remove(int id);
    void        increment(int id);
    void        decrement(int id);

    ActorID     actor_id(int id) const { return _actor_id[id]; }
    Vector3i    pos(int id) const { return _pos[id]; }
    int         health(int id) const { return _health[id]; }
    int         speed(int id) const { return _speed[id]; }
    Task&       task(int id) { return _task[id]; }
    int         count() const { return _count; }

    void        set_pos(int id, Vector3i pos);
    void        set_health(int id, int health);
    void        set_speed(int id, int speed);
    void        speed_tick(int id);
    bool        use_speed(int id, int speed);

    PathFinder& path_finder(int id) { return _path_finder[id]; }
    Path&       path(int id) { return _path[id]; }

private:
    Array<ActorID>      _actor_id;
    Array<Vector3i>     _pos;
    Array<int>          _health;
    Array<int>          _speed;
    Array<Task>         _task;
    Array<PathFinder>   _path_finder;
    Array<Path>         _path;
    Array<int>          _counter;
    Array<int>          _free;
    int                 _count;
};

#endif
