#include <cmath>
#include <game.h>
#include <tile.h>
#include <path.h>
#include <action_id.h>
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
    if (game.map.tile_at(dst.x, dst.y, dst.z) == TileID::None && game.map.floor(dst) != MaterialID::None)
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

    static const Vector3i dirs[4] = {
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

    sample_size = min(game.map.action_count(), sample_size_max);
    if (sample_size == 0)
        return;
    for (size_t i = 0; i < sample_size; ++i)
        samples.push_back(game.map.action_by_index(rand() % game.map.action_count()));
    path_finder.start(pos, distance_heuristic(pos, samples));
    for (size_t count = 0; count < nodes_max; ++count)
    {
        if (!path_finder.fetch(node))
            break;
        for (int i = 0; i < 4; ++i)
        {
            tmp = node + dirs[i];
            MapAction action = game.map.action_at(tmp.x, tmp.y, tmp.z);
            if (action != MapAction::None)
            {
                path_finder.finish_with(game.actors.path(actor), tmp);
                switch (action)
                {
                    case MapAction::Mine:
                        game.actors.set_action(actor, ActionID::Mine);
                        return;
                    case MapAction::Chop:
                        game.actors.set_action(actor, ActionID::Chop);
                        return;
                    default:
                        break;
                }
            }
            for (int j = 0; j < 3; ++j)
            {
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
    if (game.actors.action(actor) != ActionID::None)
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

static void ai_action(Game& game, int actor, ActionID action)
{
    Vector3i pos;

    pos = game.actors.path(actor).front();
    if (manhattan_distance(game.actors.pos(actor), pos) > 1)
    {
        game.actors.set_action(actor, ActionID::Wander);
        return;
    }
    game.map.set_flash(pos, Map::Flash::Action);
    if (!game.actors.use_speed(actor, 2000))
        return;
    drop_item_at(game, pos);
    game.map.set_action(pos.x, pos.y, pos.z, MapAction::None);
    game.map.set_tile(pos.x, pos.y, pos.z, TileID::None);
    game.map.set_material(pos.x, pos.y, pos.z, MaterialID::None);
    game.actors.set_action(actor, ActionID::Wander);
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
    Actors& actors = game.actors;

    count = actors.count();

    for (int i = 0; i < count; ++i)
    {
        actors.speed_tick(i);
        if (move_with_path(game, i))
            continue;
        switch (actors.action(i))
        {
            case ActionID::Wander:
                ai_wander(game, i);
                break;
            case ActionID::Mine:
                ai_action(game, i, ActionID::Mine);
                break;
            case ActionID::Chop:
                ai_action(game, i, ActionID::Chop);
                break;
            default:
                break;
        }
    }
}
