#include <engine/game.h>
#include <scene/main_menu_scene.h>
#include <scene/worldmap_generation_scene.h>

void WorldmapGenerationScene::setup()
{

}

void WorldmapGenerationScene::teardown()
{

}

void WorldmapGenerationScene::update()
{
    auto& keyboard = game().keyboard();

    if (keyboard.pressed(SDL_SCANCODE_ESCAPE))
        game().set_scene<MainMenuScene>();
}

void WorldmapGenerationScene::render(DrawBuffer& draw_buffer)
{

}
