#include <scene/ingame_scene.h>
#include <scene/main_menu_scene.h>
#include <engine/game.h>

void IngameScene::setup()
{

}

void IngameScene::teardown()
{

}

void IngameScene::update()
{
    auto& keyboard = game().keyboard();

    if (keyboard.pressed(SDL_SCANCODE_ESCAPE))
        game().set_scene<MainMenuScene>();
}

void IngameScene::render(DrawBuffer& draw_buffer)
{
    print(draw_buffer, 0, 0, "In Game!", {0, 255, 0}, {0, 0, 0});
}
