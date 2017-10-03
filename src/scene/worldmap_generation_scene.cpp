#include <engine/game.h>
#include <scene/main_menu_scene.h>
#include <scene/worldmap_generation_scene.h>
#include <worldmap_generator.h>
#include <draw.h>
#include <sys/stat.h>
#include <util/file_path.h>
#include <serialize.h>
#include <math/algorithm.h>

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
    _world_size = 256;
    generate();
}

void WorldmapGenerationScene::teardown()
{
    delete _worldmap;
}

void WorldmapGenerationScene::update()
{
    auto& input = game().input();
    InputEvent e;

    while (input.poll(e))
    {
        if (e.type == InputEventType::KeyDown)
        {
            switch (e.key.scancode)
            {
            case Keyboard::Enter:
                save_worldmap(*_worldmap);
            case Keyboard::Escape:
                game().set_scene<MainMenuScene>();
                return;
            case Keyboard::Right:
                change_size(_world_size * 2);
                break;
            case Keyboard::Left:
                change_size(_world_size / 2);
                break;
            }
        }
        else if (e.type == InputEventType::Text)
        {
            switch (e.text.unicode)
            {
            case U'r':
                generate();
                break;
            }
        }
    }
}

void WorldmapGenerationScene::render(DrawBuffer& draw_buffer)
{
    int size;
    int w;
    int h;

    w = draw_buffer.size().x;
    h = draw_buffer.size().y - 2;
    size = min(w, h);

    printf(draw_buffer, 0, 0, "World %u", {255, 255, 255}, {0, 0, 0}, _world_id);
    draw(draw_buffer, *_worldmap, {{0, 1}, {size, size}}, {{0, 0}, _worldmap->size()});
    printf(draw_buffer, 0, size + 1, "Size: %dx%d", {255, 255, 255}, {0, 0, 0}, _world_size, _world_size);
}

void WorldmapGenerationScene::generate()
{
    delete _worldmap;
    WorldmapGenerator generator;
    _worldmap = generator.generate(_world_id, {_world_size, _world_size}, game().rng());
}

void WorldmapGenerationScene::change_size(int size)
{
    if (size < 32)
        size = 32;
    else if (size > 512)
        size = 512;
    if (size != _world_size)
    {
        _world_size = size;
        generate();
    }
}
