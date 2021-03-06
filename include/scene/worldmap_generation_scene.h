#ifndef WORLDMAP_GENERATION_SCENE_H
#define WORLDMAP_GENERATION_SCENE_H

#include <engine/game_scene.h>
#include <worldmap.h>

class WorldmapGenerationScene : public GameScene
{
public:
    void setup() override;
    void teardown() override;
    void update() override;
    void render(DrawBuffer& draw_buffer) override;

private:
    void    generate();
    void    change_size(int size);

    Worldmap*   _worldmap;
    u16         _world_id;
    int         _world_size;
};

#endif
