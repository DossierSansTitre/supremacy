#ifndef EMBARK_SCENE_H
#define EMBARK_SCENE_H

#include <engine/game_scene.h>

class EmbarkScene : public GameScene
{
public:
    EmbarkScene();
    ~EmbarkScene();

    void setup() override;
    void teardown() override;
    void update() override;
    void render(DrawBuffer& db) override;

private:
};

#endif
