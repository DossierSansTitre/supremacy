#include <actors.h>
#include <actor_data.h>

Actors::Actors()
{
    _count = 0;
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
        _speed[id] = actor_data.max_speed;
        _counter[id] = 1;
        return id;
    }
    else
    {
        _actor_id.push_back(actor_id);
        _pos.push_back(pos);
        _health.push_back(actor_data.max_health);
        _speed.push_back(actor_data.max_speed);
        _counter.push_back(1);
        return _count++;
    }
}

void Actors::remove(int id)
{
    _actor_id[id] = ActorID::NONE;
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
