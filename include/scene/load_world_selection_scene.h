#ifndef LOAD_WORLD_SELECTION_SCENE_H
#define LOAD_WORLD_SELECTION_SCENE_H

#include <types.h>
#include <engine/game_scene.h>

class LoadWorldSelectionScene : public GameScene
{
public:
    void    setup() override;
    void    update() override;
    void    render(DrawBuffer& db) override;

private:
    Array<u16>  _worldmaps;
};

#endif
