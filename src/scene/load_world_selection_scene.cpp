#include <scene/load_world_selection_scene.h>
#include <scene/main_menu_scene.h>
#include <engine/game.h>

void LoadWorldSelectionScene::setup()
{

}

void LoadWorldSelectionScene::update()
{
    auto& kb = game().keyboard();

    if (kb.pressed(SDL_SCANCODE_ESCAPE))
        game().set_scene<MainMenuScene>();
}

void LoadWorldSelectionScene::render(DrawBuffer& db)
{

}
