#ifndef SCENE_MAIN_MENU_SCENE_H
#define SCENE_MAIN_MENU_SCENE_H

#include <engine/game_scene.h>

class MainMenuScene : public GameScene
{
public:
    void setup() override;
    void update() override;
    void render(DrawBuffer& draw_buffer) override;

private:
    void menu_action();

    int _selection;
};

#endif
