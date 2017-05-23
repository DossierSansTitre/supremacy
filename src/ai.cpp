#include <game.h>
#include <tile.h>

static bool can_move(Game& game, int actor, Vec3 delta)
{
    Vec3 src;
    Vec3 dst;

    src = game.actors.pos(actor);
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

static void ai_wander(Game& game, int actor)
{
    Actors& actors = game.actors;
    Vec3 delta = {0, 0, 0};
    int r;

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
