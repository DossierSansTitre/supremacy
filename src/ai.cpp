#include <cmath>
#include <game.h>
#include <tile.h>
#include <path.h>
#include <action_id.h>
#include <task.h>
#include <material.h>
#include <std/fixed_array.h>
#include <math/linear.h>

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

static bool can_move_from(Game& game, int actor, Vector3i src, Vector3i delta)
{
    (void)actor;

    Vector3i dst;

    dst = src + delta;

    if (delta.x == 0 && delta.y == 0 && delta.z == 0)
        return true;
    if (game.map.occupied(dst))
        return false;
    if (delta.z < 0)
    {
        if (!Tile::from_id(game.map.tile_at(dst.x, dst.y, dst.z)).move_up && !Tile::from_id(game.map.tile_at(dst.x, dst.y, dst.z)).move_down)
            return false;
    }
    else if (delta.z > 0)
    {
        if (!Tile::from_id(game.map.tile_at(src.x, src.y, src.z)).move_up)
            return false;
    }
    if (game.map.tile_at(dst.x, dst.y, dst.z) == TileID::None && game.map.floor(dst) != 0)
        return true;
    if (Tile::from_id(game.map.tile_at(dst.x, dst.y, dst.z)).walkable)
        return true;
    return false;

}

static bool can_move(Game& game, int actor, Vector3i delta)
{
    Vector3i src;

    src = game.actors.pos(actor);
    return can_move_from(game, actor, src, delta);
}

static bool try_move(Game& game, int actor, Vector3i delta)
{
    Vector3i src;
    Vector3i dst;
    bool b;

    src = game.actors.pos(actor);
    dst = src + delta;
    b = can_move(game, actor, delta);
    if (b)
    {
        game.actors.set_pos(actor, dst);
        game.map.set_occupied(src, false);
        game.map.set_occupied(dst, true);
    }
    return b;
}

static bool try_move_auto_slope(Game& game, int actor, Vector3i delta)
{
    static const Vector3i up = {0, 0, 1};

    if (try_move(game, actor, delta))
        return true;
    if (try_move(game, actor, delta + up))
        return true;
    if (try_move(game, actor, delta - up))
        return true;
    return false;
}

static void try_pathfind(Game& game, int actor)
{
    static const size_t nodes_max = 500;
    static const size_t sample_size_max = 25;

    static const Vector3i dirs[5] = {
        {0, 0, 0},
        {-1, 0, 0},
        {0, -1, 0},
        {1, 0, 0},
        {0, 1, 0}
    };

    static const Vector3i deltas[3] = {
        {0, 0, 0},
        {0, 0, 1},
        {0, 0, -1}
    };

    FixedArray<Vector3i, sample_size_max> samples;
    size_t sample_size;

    PathFinder& path_finder = game.actors.path_finder(actor);
    Vector3i pos = game.actors.pos(actor);
    Vector3i node;
    Vector3i delta;
    Vector3i tmp;

    sample_size = min(game.map.task_count(), sample_size_max);
    if (sample_size == 0)
        return;
    for (size_t i = 0; i < sample_size; ++i)
        samples.push_back(game.map.task_by_index(rand() % game.map.task_count()));
    path_finder.start(pos, distance_heuristic(pos, samples));
    for (size_t count = 0; count < nodes_max; ++count)
    {
        if (!path_finder.fetch(node))
            break;
        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                if (i == 0 && j == 0)
                    continue;
                tmp = node + dirs[i] + deltas[j];
                uint16_t task = game.map.task_at(tmp.x, tmp.y, tmp.z);
                if (task)
                {
                    path_finder.finish_with(game.actors.path(actor), tmp);
                    game.actors.set_task(actor, task);
                    return;
                }
                delta = dirs[i] + deltas[j];
                if (can_move_from(game, actor, node, delta))
                {
                    path_finder.explore(node + delta, distance_heuristic(node + delta, samples));
                    break;
                }
            }
        }
    }
}

static bool move_with_path(Game& game, int actor)
{
    Path& path = game.actors.path(actor);
    Vector3i next_node;
    Vector3i pos;
    Vector3i delta;

    if (path.size() < 2)
        return false;
    if (game.actors.task(actor))
        game.map.set_flash(game.actors.path(actor).front(), Map::Flash::Pending);
    if (!game.actors.use_speed(actor, 100))
        return true;
    next_node = path.back();
    path.pop_back();
    pos = game.actors.pos(actor);
    delta = next_node - pos;
    if (delta.x * delta.x + delta.y * delta.y > 1)
    {
        path.clear();
        return false;
    }
    if (!try_move(game, actor, delta))
    {
        path.clear();
        return false;
    }
    return true;
}

void drop_item_at(Game& game, Vector3i pos)
{
    int frequency = Tile::from_id(game.map.tile_at(pos.x, pos.y, pos.z)).dropping_frequency;
    if (frequency != 0 && rand() % frequency == 0)
        game.items.add(Material::from_id(game.map.material_at(pos.x, pos.y, pos.z)).dropping_item, pos, 1);
}

static void ai_task(Game& game, int actor, uint16_t task)
{
    Vector3i pos;
    Vector3i delta;

    pos = game.actors.path(actor).front();
    delta = pos - game.actors.pos(actor);
    if (delta.x * delta.x + delta.y * delta.y > 1 || delta.z * delta.z > 1)
    {
        game.actors.set_task(actor, 0);
        return;
    }
    game.map.set_flash(pos, Map::Flash::Action);
    if (!game.actors.use_speed(actor, 2000))
        return;
    const Task& task_data = Task::from_id(task);
    if (task_data.into == 0)
    {
        drop_item_at(game, pos);
        game.map.set_tile(pos.x, pos.y, pos.z, TileID::None);
        game.map.set_material(pos.x, pos.y, pos.z, 0);
    }
    else
    {
        const Tile& tile_data = Tile::from_id(TileID(task_data.into));

        if (game.map.material_at(pos.x, pos.y, pos.z) == 0)
            game.map.set_material(pos.x, pos.y, pos.z, game.map.floor(pos));
        game.map.set_tile(pos.x, pos.y, pos.z, TileID(task_data.into));
        if (tile_data.move_up)
            game.map.set_floor(pos + Vector3i(0, 0, 1), MaterialID(0));
        if (tile_data.move_down)
            game.map.set_floor(pos, MaterialID(0));
    }
    game.map.set_task(pos.x, pos.y, pos.z, 0);
    game.actors.set_task(actor, 0);
    try_pathfind(game, actor);
}

static void ai_wander(Game& game, int actor)
{
    Actors& actors = game.actors;
    Vector3i delta = {0, 0, 0};
    int r;

    if ((actor + game.tick) % 16 == 0)
    {
        try_pathfind(game, actor);
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
    try_move_auto_slope(game, actor, delta);
}

void game_ai(Game& game)
{
    int count;
    uint16_t task;
    Actors& actors = game.actors;

    count = actors.count();

    for (int i = 0; i < count; ++i)
    {
        actors.speed_tick(i);
        if (move_with_path(game, i))
            continue;
        task = actors.task(i);
        if (task)
            ai_task(game, i, task);
        else
            ai_wander(game, i);
    }
}
