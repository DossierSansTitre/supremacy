#include <scene/ingame_scene.h>
#include <scene/main_menu_scene.h>
#include <engine/game.h>

void IngameScene::setup()
{
    _world = new World({1024, 1024, 64});
    generate_map(_world->map, 0x12345678);
    _world->camera.z = 45;
}

void IngameScene::teardown()
{
    delete _world;
}

void IngameScene::update()
{
    auto& keyboard = game().keyboard();

    if (keyboard.pressed(SDL_SCANCODE_ESCAPE))
        game().set_scene<MainMenuScene>();
}

void IngameScene::render(DrawBuffer& draw_buffer)
{
    draw_world(draw_buffer, *_world, game());
}
