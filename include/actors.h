#ifndef ACTORS_H
#define ACTORS_H

#include <vector>
#include <actor_id.h>
#include <action_id.h>
#include <math/vector.h>
#include <path_finder.h>
#include <path.h>

class Actors
{
public:
    Actors();
    ~Actors();

    int         add(ActorID actor_id, Vector3i pos);
    void        remove(int id);
    void        increment(int id);
    void        decrement(int id);

    ActorID     actor_id(int id) const { return _actor_id[id]; }
    Vector3i        pos(int id) const { return _pos[id]; }
    int         health(int id) const { return _health[id]; }
    int         speed(int id) const { return _speed[id]; }
    ActionID    action(int id) const { return _action[id]; }
    int         count() const { return _count; }

    void        set_pos(int id, Vector3i pos);
    void        set_health(int id, int health);
    void        set_speed(int id, int speed);
    void        speed_tick(int id);
    bool        use_speed(int id, int speed);
    void        set_action(int id, ActionID action);

    PathFinder& path_finder(int id) { return _path_finder[id]; }
    Path&       path(int id) { return _path[id]; }

private:
    std::vector<ActorID>    _actor_id;
    std::vector<Vector3i>       _pos;
    std::vector<int>        _health;
    std::vector<int>        _speed;
    std::vector<ActionID>   _action;
    std::vector<PathFinder> _path_finder;
    std::vector<Path>       _path;
    std::vector<int>        _counter;
    std::vector<int>        _free;
    int                     _count;
};

#endif
