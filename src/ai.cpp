#include <cmath>
#include <game.h>
#include <tile.h>
#include <path.h>
#include <action_id.h>
#include <material.h>

uint32_t manhattan(Vec3 a, Vec3 b)
{
    a -= b;

    if (a.x < 0)
        a.x = -a.x;
    if (a.y < 0)
        a.y = -a.y;
    if (a.z < 0)
        a.z = -a.z;

    return a.x + a.y + a.z;
}

uint32_t manhattan(size_t count, Vec3* multi, Vec3 pos)
{
    uint32_t value;
    uint32_t tmp;

    value = manhattan(pos, multi[0]);
    for (size_t i = 1; i < count; ++i)
    {
        tmp = manhattan(pos, multi[i]);
        if (tmp < value)
            value = tmp;
    }
    return value;
}

static bool can_move_from(Game& game, int actor, Vec3 src, Vec3 delta)
{
    Vec3 dst;

    dst = src + delta;

    if (delta.x == 0 && delta.y == 0 && delta.z == 0)
        return true;
    if (game.map.occupied(dst))
        return false;
    if (delta.z < 0)
    {
        if (game.map.tile_at(dst.x, dst.y, dst.z) != TileID::Ramp)
            return false;
    }
    else if (delta.z > 0)
    {
        if (game.map.tile_at(src.x, src.y, src.z) != TileID::Ramp)
            return false;
    }
    if (Tile::from_id(game.map.tile_at(dst.x, dst.y, dst.z)).walkable)
        return true;
    return false;

}

static bool can_move(Game& game, int actor, Vec3 delta)
{
    Vec3 src;

    src = game.actors.pos(actor);
    return can_move_from(game, actor, src, delta);
}

static bool try_move(Game& game, int actor, Vec3 delta)
{
    Vec3 src;
    Vec3 dst;
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

static bool try_move_auto_slope(Game& game, int actor, Vec3 delta)
{
    static const Vec3 up = {0, 0, 1};

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

    static const Vec3 dirs[4] = {
        {-1, 0, 0},
        {0, -1, 0},
        {1, 0, 0},
        {0, 1, 0}
    };

    static const Vec3 deltas[3] = {
        {0, 0, 0},
        {0, 0, 1},
        {0, 0, -1}
    };

    size_t sample_size;
    Vec3 samples[sample_size_max];

    PathFinder& path_finder = game.actors.path_finder(actor);
    Vec3 pos = game.actors.pos(actor);
    Vec3 node;
    Vec3 delta;
    Vec3 tmp;

    sample_size = game.map.action_count();
    if (sample_size == 0)
        return;
    else if (sample_size > sample_size_max)
        sample_size = sample_size_max;

    for (size_t i = 0; i < sample_size; ++i)
        samples[i] = game.map.action_by_index(rand() % game.map.action_count());

    path_finder.start(pos, manhattan(sample_size, samples, pos));
    for (size_t count = 0; count < nodes_max; ++count)
    {
        if (!path_finder.fetch(node))
            break;
        for (int i = 0; i < 4; ++i)
        {
            tmp = node + dirs[i];
            if (game.map.action_at(tmp.x, tmp.y, tmp.z) != MapAction::None)
            {
                path_finder.finish_with(game.actors.path(actor), tmp);
                game.actors.set_action(actor, ActionID::Mine);
                return;
            }
            for (int j = 0; j < 3; ++j)
            {
                delta = dirs[i] + deltas[j];
                if (can_move_from(game, actor, node, delta))
                {
                    path_finder.explore(node + delta, manhattan(sample_size, samples, node + delta));
                    break;
                }
            }
        }
    }
}

static bool move_with_path(Game& game, int actor)
{
    Path& path = game.actors.path(actor);
    Vec3 next_node;
    Vec3 pos;
    Vec3 delta;

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

void drop_item_at(Game& game, Vec3 pos)
{
    int frequency = Tile::from_id(game.map.tile_at(pos.x, pos.y, pos.z)).dropping_frequency;
    if (frequency != 0 && rand() % frequency == 0)
        game.items.add(Material::from_id(game.map.material_at(pos.x, pos.y, pos.z)).dropping_item, pos, 1);
}

static void ai_mine(Game& game, int actor)
{
    Vec3 pos;

    pos = game.actors.path(actor).front();
    if (manhattan(game.actors.pos(actor), pos) > 1)
    {
        game.actors.set_action(actor, ActionID::Wander);
        return;
    }
    game.map.set_flash(pos, Map::Flash::Action);
    if (!game.actors.use_speed(actor, 2000))
        return;
    drop_item_at(game, pos);
    game.map.set_action(pos.x, pos.y, pos.z, MapAction::None);
    game.map.set_tile(pos.x, pos.y, pos.z, TileID::Floor);
    if (game.map.material_at(pos.x, pos.y, pos.z) == MaterialID::Grass)
        game.map.set_material(pos.x, pos.y, pos.z, MaterialID::Dirt);
    game.actors.set_action(actor, ActionID::Wander);
    try_pathfind(game, actor);
}

static void ai_wander(Game& game, int actor)
{
    Actors& actors = game.actors;
    Vec3 delta = {0, 0, 0};
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
                ai_mine(game, i);
                break;
            default:
                break;
        }
    }
}
