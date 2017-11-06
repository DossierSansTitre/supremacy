#ifndef GAME_PHYSICS_H
#define GAME_PHYSICS_H

#include <math/vector.h>
#include <path_finder.h>
#include <world.h>

static const Vector3i delta_permutations[] = {
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

struct World;
bool can_move_from(World& world, int actor, Vector3i src, Vector3i delta);
bool can_act_upon(World& world, int actor, Vector3i src, Vector3i delta);
bool can_move(World& world, int actor, Vector3i delta);
bool try_move(World& world, int actor, Vector3i delta);

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

template <typename Container, typename Predicate>
bool pathfind(Path& out, World& world, int actor, const Container& targets, Predicate pred)
{
    static const size_t nodes_max = 500;
    auto& actors = world.actors;
    auto& map = world.map;

    PathFinder path_finder;
    Vector3i pos = actors.pos(actor);
    Vector3i node;
    Vector3i delta;
    Vector3i dst;

    path_finder.start(pos, distance_heuristic(pos, targets));
    for (size_t count = 0; count < nodes_max; ++count)
    {
        if (!path_finder.fetch(node))
            break;
        for (int i = 0; i < 14; ++i)
        {
            delta = delta_permutations[i];
            dst = node + delta;
            if (pred(world, actor, node, delta))
            {
                path_finder.finish_with(out, dst);
                return true;
            }
            if (can_move_from(world, actor, node, delta))
                path_finder.explore(dst, distance_heuristic(dst, targets));
        }
    }
    return false;
}

template <typename Container>
bool pathfind_move(Path& out, World& world, int actor, const Container& targets)
{
    return pathfind(out, world, actor, targets, &can_move_from);
}

template <typename Container>
bool pathfind_act(Path& out, World& world, int actor, const Container& targets)
{
    return pathfind(out, world, actor, targets, &can_act_upon);
}

#endif
