#include <game/physics.h>
#include <world.h>
#include <tile.h>

bool can_move_from(World& world, int actor, Vector3i src, Vector3i delta)
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

bool can_act_upon(World& world, Vector3i src, Vector3i dst)
{
    Vector3i delta = dst - src;

    if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z > 1)
        return false;
    if (src == dst)
        return true;
    if (delta.z == 0)
        return true;
    if (!world.map.floor(src) && delta.z < 0)
        return true;
    if (!world.map.floor(dst) && delta.z > 0)
        return true;
    return false;
}

bool can_move(World& world, int actor, Vector3i delta)
{
    Vector3i src;

    src = world.actors.pos(actor);
    return can_move_from(world, actor, src, delta);
}

bool try_move(World& world, int actor, Vector3i delta)
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
