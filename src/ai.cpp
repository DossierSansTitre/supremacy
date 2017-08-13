#include <cmath>
#include <tile.h>
#include <path.h>
#include <action_id.h>
#include <task.h>
#include <material.h>
#include <std/fixed_array.h>
#include <math/linear.h>
#include <update.h>
#include <world.h>

template <typename Container>
uint32_t distance_heuristic(Vector3i pos, const Container& container)
{
    uint32_t min;
    uint32_t tmp;
    auto end = container.end();

    min = manhattan_distance(pos, container.front());

    for (auto it = container.begin() + 1; it != end; ++it)
    {
        tmp = manhattan_distance(pos, *it);
        if (tmp < min)
            min = tmp;
    }
    return min;
}

static bool can_move_from(World& world, int actor, Vector3i src, Vector3i delta)
{
    (void)actor;

    Vector3i dst;
    auto& map = world.map;

    dst = src + delta;

    if (delta.x == 0 && delta.y == 0 && delta.z == 0)
        return true;
    if (map.occupied(dst))
        return false;
    if (delta.z < 0)
    {
        if (!Tile::from_id(map.tile_at(dst.x, dst.y, dst.z)).move_up && !Tile::from_id(map.tile_at(dst.x, dst.y, dst.z)).move_down)
            return false;
    }
    else if (delta.z > 0)
    {
        if (!Tile::from_id(map.tile_at(src.x, src.y, src.z)).move_up)
            return false;
    }
    if (!map.tile_at(dst.x, dst.y, dst.z) && map.floor(dst))
        return true;
    if (Tile::from_id(map.tile_at(dst.x, dst.y, dst.z)).walkable)
        return true;
    return false;
}

static bool can_move(World& world, int actor, Vector3i delta)
{
    Vector3i src;

    src = world.actors.pos(actor);
    return can_move_from(world, actor, src, delta);
}

static bool try_move(World& world, int actor, Vector3i delta)
{
    Vector3i src;
    Vector3i dst;
    bool b;

    src = world.actors.pos(actor);
    dst = src + delta;
    b = can_move(world, actor, delta);
    if (b)
    {
        world.actors.set_pos(actor, dst);
        world.map.set_occupied(src, false);
        world.map.set_occupied(dst, true);
    }
    return b;
}

static bool try_move_auto_slope(World& world, int actor, Vector3i delta)
{
    static const Vector3i up = {0, 0, 1};

    if (try_move(world, actor, delta))
        return true;
    if (try_move(world, actor, delta + up))
        return true;
    if (try_move(world, actor, delta - up))
        return true;
    return false;
}

static void try_pathfind(World& world, int actor)
{
    static const size_t nodes_max = 500;
    static const size_t sample_size_max = 25;

    static const Vector3i deltas[14] = {
        {-1, 0, 0},
        {0, -1, 0},
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, -1},
        {-1, 0, -1},
        {0, -1, -1},
        {1, 0, -1},
        {0, 1, -1},
        {0, 0, 1},
        {-1, 0, 1},
        {0, -1, 1},
        {1, 0, 1},
        {0, 1, 1},
    };

    FixedArray<Vector3i, sample_size_max> samples;
    size_t sample_size;

    auto& actors = world.actors;
    auto& map = world.map;

    PathFinder& path_finder = actors.path_finder(actor);
    Vector3i pos = actors.pos(actor);
    Vector3i node;
    Vector3i delta;
    Vector3i dst;

    sample_size = min(map.task_count(), sample_size_max);
    if (sample_size == 0)
        return;
    for (size_t i = 0; i < sample_size; ++i)
        samples.push_back(map.task_by_index(rand() % map.task_count()));
    path_finder.start(pos, distance_heuristic(pos, samples));
    for (size_t count = 0; count < nodes_max; ++count)
    {
        if (!path_finder.fetch(node))
            break;
        for (int i = 0; i < 14; ++i)
        {
            delta = deltas[i];
            dst = node + delta;
            uint16_t task = map.task_at(dst.x, dst.y, dst.z);
            if (task)
            {
                path_finder.finish_with(actors.path(actor), dst);
                actors.set_task(actor, task);
                return;
            }
            if (can_move_from(world, actor, node, delta))
            {
                path_finder.explore(dst, distance_heuristic(dst, samples));
            }
        }
    }
}

static bool move_with_path(World& world, int actor)
{
    auto& actors = world.actors;
    Path& path = actors.path(actor);
    Vector3i next_node;
    Vector3i pos;
    Vector3i delta;

    if (path.size() < 2)
        return false;
    if (actors.task(actor))
        world.map.set_flash(actors.path(actor).front(), Map::Flash::Pending);
    if (!actors.use_speed(actor, 100))
        return true;
    next_node = path.back();
    path.pop_back();
    pos = actors.pos(actor);
    delta = next_node - pos;
    if (delta.x * delta.x + delta.y * delta.y > 1)
    {
        path.clear();
        return false;
    }
    if (!try_move(world, actor, delta))
    {
        path.clear();
        return false;
    }
    return true;
}

void drop_item_at(World& world, Vector3i pos)
{
    auto& map = world.map;
    auto& items = world.items;

    int frequency = Tile::from_id(map.tile_at(pos.x, pos.y, pos.z)).dropping_frequency;
    if (frequency != 0 && rand() % frequency == 0)
        items.add(Material::from_id(map.material_at(pos.x, pos.y, pos.z)).dropping_item, pos, 1);
}

static void ai_task(World& world, int actor, uint16_t task)
{
    Vector3i pos;
    Vector3i delta;

    auto& actors = world.actors;
    auto& map = world.map;

    pos = actors.path(actor).front();
    delta = pos - actors.pos(actor);
    if (delta.x * delta.x + delta.y * delta.y > 1 || delta.z * delta.z > 1)
    {
        actors.set_task(actor, 0);
        return;
    }
    map.set_flash(pos, Map::Flash::Action);
    if (!actors.use_speed(actor, 2000))
        return;
    const Task& task_data = Task::from_id(task);
    if (task_data.into == 0)
    {
        drop_item_at(world, pos);
        map.set_tile(pos.x, pos.y, pos.z, 0);
        map.set_material(pos.x, pos.y, pos.z, 0);
    }
    else
    {
        const Tile& tile_data = Tile::from_id(TileID(task_data.into));

        if (map.material_at(pos.x, pos.y, pos.z) == 0)
            map.set_material(pos.x, pos.y, pos.z, map.floor(pos));
        map.set_tile(pos.x, pos.y, pos.z, TileID(task_data.into));
        if (tile_data.move_up)
            map.set_floor(pos + Vector3i(0, 0, 1), MaterialID(0));
        if (tile_data.move_down)
            map.set_floor(pos, MaterialID(0));
    }
    map.set_task(pos.x, pos.y, pos.z, 0);
    actors.set_task(actor, 0);
    try_pathfind(world, actor);
}

static void ai_wander(World& world, int actor, u32 tick)
{
    Vector3i delta = {0, 0, 0};
    int r;

    auto& actors = world.actors;

    if ((actor + tick) % 16 == 0)
    {
        try_pathfind(world, actor);
        return;
    }
    if (!actors.use_speed(actor, 250))
        return;

    r = rand() % 4;

    if (r == 0)
        delta.x++;
    else if (r == 1)
        delta.x--;
    else if (r == 2)
        delta.y++;
    else
        delta.y--;
    try_move_auto_slope(world, actor, delta);
}

void update_ai(World& world, u32 tick)
{
    int count;
    uint16_t task;
    Actors& actors = world.actors;

    count = actors.count();

    for (int i = 0; i < count; ++i)
    {
        actors.speed_tick(i);
        if (move_with_path(world, i))
            continue;
        task = actors.task(i);
        if (task)
            ai_task(world, i, task);
        else
            ai_wander(world, i, tick);
    }
}
