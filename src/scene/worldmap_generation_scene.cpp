#include <engine/game.h>
#include <scene/main_menu_scene.h>
#include <scene/worldmap_generation_scene.h>
#include <worldmap_generator.h>
#include <draw.h>

void WorldmapGenerationScene::setup()
{
    _worldmap = nullptr;
    generate();
}

void WorldmapGenerationScene::teardown()
{
    delete _worldmap;
}

void WorldmapGenerationScene::update()
{
    auto& keyboard = game().keyboard();

    if (keyboard.pressed(SDL_SCANCODE_ESCAPE))
        game().set_scene<MainMenuScene>();
    else if (keyboard.key_pressed(SDLK_r))
        generate();
}

void WorldmapGenerationScene::render(DrawBuffer& draw_buffer)
{
    draw_worldmap(draw_buffer, *_worldmap, Vector2i(1, 1));
}

void WorldmapGenerationScene::generate()
{
    delete _worldmap;
    WorldmapGenerator generator;
    _worldmap = generator.generate({128, 64});
}
