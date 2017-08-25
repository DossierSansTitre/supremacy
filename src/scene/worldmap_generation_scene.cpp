#include <engine/game.h>
#include <scene/main_menu_scene.h>
#include <scene/worldmap_generation_scene.h>
#include <worldmap_generator.h>
#include <draw.h>
#include <sys/stat.h>
#include <util/file_path.h>
#include <serialize.h>

static u16 get_next_world_id()
{
    u16 id;
    char buffer[4096];
    struct stat s;

    id = 1;
    for (;;)
    {
        snprintf(buffer, 4096, "worlds/world_%05u", id);
        if (stat(save_path(buffer), &s))
            return id;
        id++;
    }
}

void WorldmapGenerationScene::setup()
{
    _worldmap = nullptr;
    _world_id = get_next_world_id();
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
    {
        game().set_scene<MainMenuScene>();
    }
    else if (keyboard.pressed(SDL_SCANCODE_RETURN))
    {
        save_worldmap(*_worldmap);
        game().set_scene<MainMenuScene>();
    }
    else if (keyboard.key_pressed(SDLK_r))
    {
        generate();
    }
}

void WorldmapGenerationScene::render(DrawBuffer& draw_buffer)
{
    printf(draw_buffer, 1, 0, "World %u", {255, 255, 255}, {0, 0, 0}, _world_id);
    draw_worldmap(draw_buffer, *_worldmap, Vector2i(1, 1));
}

void WorldmapGenerationScene::generate()
{
    delete _worldmap;
    WorldmapGenerator generator;
    _worldmap = generator.generate(_world_id, {128, 64}, game().rng());
}
