#include <actors.h>
#include <actor_data.h>

Actors::Actors()
{
}

Actors::~Actors()
{
}

void Actors::add(ActorID actor_id, Vec3 pos)
{
    const ActorData actor_data = ActorData::from_id(actor_id);
    
    _actor_id.push_back(actor_id);
    _pos.push_back(pos);
    _health.push_back(actor_data.max_health);
    _speed.push_back(actor_data.max_speed);
}

void Actors::remove(int id)
{
    _actor_id.erase(_actor_id.begin() + id);
    _pos.erase(_pos.begin() + id);
    _health.erase(_health.begin() + id);
    _speed.erase(_speed.begin() + id);
}

void Actors::set_pos(int id, Vec3 pos)
{
    _pos[id] = pos;
}

void Actors::set_health(int id, float health)
{
    _health[id] = health;
}

void Actors::set_speed(int id, float speed)
{
    _speed[id] = speed;
}
