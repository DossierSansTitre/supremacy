#ifndef SCENE_INGAME_SCENE_H
#define SCENE_INGAME_SCENE_H

#include <engine/game_scene.h>
#include <world.h>

class IngameScene : public GameScene
{
public:
    void setup() override;
    void teardown() override;
    void update() override;
    void render(DrawBuffer& draw_buffer) override;

private:
    World*  _world;
};

#endif