#ifndef EMBARK_SCENE_H
#define EMBARK_SCENE_H

#include <types.h>
#include <worldmap.h>
#include <engine/game_scene.h>

class EmbarkScene : public GameScene
{
public:
    EmbarkScene(u16 world_id);
    ~EmbarkScene();

    void setup() override;
    void teardown() override;
    void update() override;
    void render(DrawBuffer& db) override;

private:
    void move_cursor(Vector2i delta);
    void fix_camera();
    bool embark();

    u16         _world_id;
    Worldmap*   _worldmap;
    Vector2i    _cursor;
    Vector2i    _camera;
    int         _width;
};

#endif
