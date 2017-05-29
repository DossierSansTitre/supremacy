#include <cstdlib>
#include <actors.h>
#include <actor_data.h>

Actors::Actors()
: _count(0)
{

}

Actors::~Actors()
{

}

int Actors::add(ActorID actor_id, Vec3 pos)
{
    const ActorData actor_data = ActorData::from_id(actor_id);

    if (_free.size() > 0)
    {
        int id = _free.back();
        _free.pop_back();
        _actor_id[id] = actor_id;
        _pos[id] = pos;
        _health[id] = actor_data.max_health;
        _action[id] = ActionID::Wander;
        _speed[id] = rand() % (10 * actor_data.speed + 1);
        _counter[id] = 1;
        _path_finder[id].reset();
        _path[id].clear();
        return id;
    }
    else
    {
        _actor_id.push_back(actor_id);
        _pos.push_back(pos);
        _action.push_back(ActionID::Wander);
        _health.push_back(actor_data.max_health);
        _speed.push_back(rand() % (10 * actor_data.speed + 1));
        _counter.push_back(1);
        _path_finder.resize(_count + 1);
        _path.resize(_count + 1);
        return _count++;
    }
}

void Actors::remove(int id)
{
    _actor_id[id] = ActorID::None;
    _free.push_back(id);
}

void Actors::increment(int id)
{
    ++_counter[id];
}

void Actors::decrement(int id)
{
    if (--_counter[id] <= 0)
        remove(id);
}

void Actors::set_pos(int id, Vec3 pos)
{
    _pos[id] = pos;
}

void Actors::set_health(int id, int health)
{
    _health[id] = health;
}

void Actors::set_speed(int id, int speed)
{
    _speed[id] = speed;
}

void Actors::speed_tick(int id)
{
    const ActorData& ad = ActorData::from_id(_actor_id[id]);
    _speed[id] += ad.speed;
}

bool Actors::use_speed(int id, int speed)
{
    if (_speed[id] >= speed)
    {
        _speed[id] -= speed;
        return true;
    }
    return false;
}

void Actors::set_action(int id, ActionID action)
{
    _action[id] = action;
}
