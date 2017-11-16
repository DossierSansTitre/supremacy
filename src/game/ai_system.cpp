#include <game/ai_system.h>
#include <game/physics.h>
#include <math/algebra.h>
#include <game/resource/task_terrain_data.h>
#include <world.h>
#include <tile.h>

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
    if (assign_task_terrain(actor))
        return true;
    return false;
}

bool AISystem::assign_task_terrain(int actor)
{
    static const size_t sample_max = 16;

    auto& map = _world->map;
    auto& task = _world->actors.task(actor);

    size_t task_count = map.task_count();
    size_t sample_count;
    Array<Vector3i> samples;

    if (task_count == 0)
        return false;

    sample_count = min(task_count, sample_max);
    for (size_t i = 0; i < sample_count; ++i)
    {
        Vector3i target;
        uint16_t id;

        target = map.task_by_index(rand() % task_count);
        id = map.task_at(target);
        if (id)
            samples.push_back(target);
    }
    if (samples.empty())
        return false;
    if (pathfind_act(task.path, *_world, actor, samples))
    {
        Vector3i dst;
        uint16_t terrain_task_id;

        dst = task.path[0];
        terrain_task_id = map.task_at(dst);
        task.type = TaskType::Terrain;
        task.terrain_task_id = terrain_task_id;
        task.target = dst;
        return true;
    }
    return false;
}

void AISystem::run_task(int actor)
{
    auto& task = _world->actors.task(actor);

    if (task.type == TaskType::Terrain)
        run_task_terrain(actor);
}

void AISystem::run_task_terrain(int actor)
{
    auto& task = _world->actors.task(actor);
    auto& map = _world->map;
    uint16_t terrain_task_id;
    Vector3i pos;

    terrain_task_id = _world->map.task_at(task.target);
    if (terrain_task_id != task.terrain_task_id)
    {
        task.type = TaskType::None;
        return;
    }
    if (can_act_upon_immediate(*_world, actor, task.target))
    {
        _world->map.set_flash(task.target, Map::Flash::Action);
        if (_world->actors.use_speed(actor, 2000))
        {
            const auto& task_terrain_data = TaskTerrainData::from_id(task.terrain_task_id);
            if (task_terrain_data.into == 0)
            {
                //drop_item_at(world, pos);
                map.set_tile(task.target, 0);
                map.set_material(task.target, 0);
            }
            else
            {
                const auto& tile_data = Tile::from_id(TileID(task_terrain_data.into));

                if (map.material_at(task.target) == 0)
                    map.set_material(task.target, map.floor(task.target));
                map.set_tile(task.target, TileID(task_terrain_data.into));
                if (tile_data.move_up)
                    map.set_floor(task.target + Vector3i(0, 0, 1), MaterialID(0));
                if (tile_data.move_down)
                    map.set_floor(task.target, MaterialID(0));
            }
            map.set_task(task.target, 0);
            task.type = TaskType::None;
        }
    }
    else
    {
        if (task.path.empty())
        {
            task.type = TaskType::None;
            return;
        }
        _world->map.set_flash(task.target, Map::Flash::Pending);
        if (_world->actors.use_speed(actor, 20))
        {
            Vector3i step;

            step = task.path.back();
            task.path.pop_back();
            if (!try_move_immediate(*_world, actor, step))
            {
                task.type = TaskType::None;
                return;
            }
        }
    }
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
