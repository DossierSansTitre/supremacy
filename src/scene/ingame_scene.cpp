#include <scene/ingame_scene.h>
#include <scene/main_menu_scene.h>
#include <engine/game.h>
#include <update.h>
#include <tile.h>

static int find_suitable_height(const Map& map, int x, int y)
{
    int depth = map.depth();

    for (int k = depth - 1; k >= 0; --k)
    {
        if (map.tile_at(x, y, k))
        {
            if (!Tile::from_id(map.tile_at(x, y, k)).walkable)
                k++;
            return k;
        }
    }
    return -1;
}

static void generate_dwarfs(World& world)
{
    int w;
    int h;
    int x;
    int y;
    int z;
    int spawn_count;

    w = world.map.width();
    h = world.map.height();

    spawn_count = 0;

    while (spawn_count < 7)
    {
        x = (w / 2) + (rand() % 16) - 8;
        y = (h / 2) + (rand() % 16) - 8;
        z = find_suitable_height(world.map, x, y);
        world.actors.add(ActorID::Dwarf, {x, y, z});
        spawn_count++;
    }
    world.camera.x = x - 20;
    world.camera.y = y - 20;
    world.camera.z = z;
}

void IngameScene::setup()
{
    _world = new World({1024, 1024, 64});
    generate_map(_world->map, rand());
    generate_dwarfs(*_world);
    _update_tick = 0;
    _render_tick = 0;
}

void IngameScene::teardown()
{
    delete _world;
}

void IngameScene::update()
{
    auto& keyboard = game().keyboard();

    if (keyboard.pressed(SDL_SCANCODE_ESCAPE))
    {
        game().set_scene<MainMenuScene>();
        return;
    }

    _world->map.tick();
    update_ai(*_world, _update_tick);
    update_ui(*_world, keyboard, _selection, _viewport);

    _update_tick++;
    _render_tick++;
}

void IngameScene::render(DrawBuffer& draw_buffer)
{
    _viewport = draw_buffer.size();
    draw_world(draw_buffer, *_world, game(), _render_tick);
    draw_selection(draw_buffer, *_world, _selection, _render_tick);
}
