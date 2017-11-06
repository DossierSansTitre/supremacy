#ifndef GAME_AI_SYSTEM_H
#define GAME_AI_SYSTEM_H

#include <math/vector.h>

struct World;

class AISystem
{
public:
    AISystem() : _world(nullptr) {}
    AISystem(World* world) : _world(world) {}

    void run();

private:
    void tick(int actor);
    bool assign_task(int actor);
    void run_task(int actor);
    void wander(int actor);
    bool move(int actor, Vector3i delta);

    World*  _world;
};

#endif
