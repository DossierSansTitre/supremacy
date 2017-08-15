#ifndef WORLDMAP_GENERATION_SCENE_H
#define WORLDMAP_GENERATION_SCENE_H

#include <engine/game_scene.h>

class WorldmapGenerationScene : public GameScene
{
public:
    void setup() override;
    void teardown() override;
    void update() override;
    void render(DrawBuffer& draw_buffer) override;
};

#endif
