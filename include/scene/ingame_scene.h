#ifndef SCENE_INGAME_SCENE_H
#define SCENE_INGAME_SCENE_H

#include <engine/game_scene.h>
#include <world.h>
#include <selection.h>
#include <draw.h>
#include <game/ai_system.h>

class IngameScene : public GameScene
{
public:
    IngameScene(u16 world_id, u32 region_id);
    void setup() override;
    void teardown() override;
    void update() override;
    void render(DrawBuffer& draw_buffer) override;

private:
    World*      _world;
    AISystem    _ai;
    Selection   _selection;
    u32         _update_tick;
    u32         _render_tick;
    Vector2u    _viewport;
};

#endif
