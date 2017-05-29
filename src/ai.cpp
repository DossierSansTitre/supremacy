#include <game.h>
#include <tile.h>

static bool can_move_from(Game& game, int actor, Vec3 src, Vec3 delta)
{
    Vec3 dst;

    dst = src + delta;

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
    Vec3 dst;
    bool b;

    dst = game.actors.pos(actor) + delta;
    b = can_move(game, actor, delta);
    if (b)
        game.actors.set_pos(actor, dst);
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
    static size_t nodes_max = 500;

    static Vec3 dirs[4] = {
        {-1, 0, 0},
        {0, -1, 0},
        {1, 0, 0},
        {0, 1, 0}
    };

    static Vec3 deltas[3] = {
        {0, 0, 0},
        {0, 0, 1},
        {0, 0, -1}
    };

    PathFinder& path_finder = game.actors.path_finder(actor);
    Vec3 pos = game.actors.pos(actor);
    Vec3 node;
    Vec3 delta;
    Vec3 tmp;

    path_finder.start(pos);
    for (size_t count = 0; count < nodes_max; ++count)
    {
        if (!path_finder.fetch(node))
            break;
        for (int i = 0; i < 4; ++i)
        {
            tmp = node + dirs[i];
            if (game.map.action_at(tmp.x, tmp.y, tmp.z) != MapAction::None)
            {
                path_finder.finish();
                return;
            }
            for (int j = 0; j < 3; ++j)
            {
                delta = dirs[i] + deltas[j];
                if (can_move_from(game, actor, node, delta))
                {
                    path_finder.explore(node + delta);
                    break;
                }
            }
        }
    }
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
        switch (actors.action(i))
        {
            case ActionID::Wander:
                ai_wander(game, i);
                break;
            default:
                break;
        }
    }
}
