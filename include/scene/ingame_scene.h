#ifndef SCENE_INGAME_SCENE_H
#define SCENE_INGAME_SCENE_H

#include <engine/game_scene.h>
#include <world.h>
#include <selection.h>
#include <draw.h>

class IngameScene : public GameScene
{
public:
    IngameScene(u16 world_id, u32 region_id);
    void setup() override;
    void teardown() override;
    void update() override;
    void render(DrawBuffer& draw_buffer) override;

private:
    u16         _world_id;
    u32         _region_id;
    World*      _world;
    Selection   _selection;
    u32         _update_tick;
    u32         _render_tick;
    Vector2u    _viewport;
};

#endif
