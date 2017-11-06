#include <game/ai_system.h>
#include <world.h>
#include <game/physics.h>

void AISystem::run()
{
    auto& actors = _world->actors;
    for (int i = 0; i < actors.count(); ++i)
    {
        actors.speed_tick(i);
        if (actors.actor_id(i) == ActorID::None)
            continue;
        tick(i);
    }
}

void AISystem::tick(int actor)
{
    auto& actors = _world->actors;
    if (actors.task(actor).type == TaskType::None && !assign_task(actor))
        wander(actor);
    else
        run_task(actor);
}

bool AISystem::assign_task(int actor)
{

    return false;
}

void AISystem::run_task(int actor)
{

}

void AISystem::wander(int actor)
{
    static const int delta_count = sizeof(delta_permutations) / sizeof(*delta_permutations);

    Vector3i delta;
    delta = delta_permutations[rand() % delta_count];
    move(actor, delta);
}

bool AISystem::move(int actor, Vector3i delta)
{
    if (!_world->actors.use_speed(actor, 250))
        return false;
    return try_move(*_world, actor, delta);
}
