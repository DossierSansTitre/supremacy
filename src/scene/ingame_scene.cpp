#include <scene/ingame_scene.h>
#include <scene/main_menu_scene.h>
#include <engine/game.h>
#include <update.h>
#include <serialize.h>
#include <worldmap.h>
#include <tile.h>
#include <util/save_helper.h>
#include <core/file_system.h>

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

IngameScene::IngameScene(u16 world_id, u32 region_id)
{
    const char* region_path;
    Worldmap* worldmap;
    Vector2i region;
    BiomeID biome_id;

    region_path = save_path_region(world_id, region_id);
    if (FileSystem::file_exists(region_path))
    {
        _world = load_world(world_id, region_id);
    }
    else
    {
        worldmap = load_worldmap(world_id);
        region.x = region_id % worldmap->size().x;
        region.y = region_id / worldmap->size().x;
        biome_id = worldmap->biome(region);
        delete worldmap;

        _world = new World(world_id, region_id, {1024, 1024, 64});
        generate_map(_world->map, biome_id, rand());
        generate_dwarfs(*_world);
    }
    _ai = AISystem(_world);
}

void IngameScene::setup()
{
    _update_tick = 0;
    _render_tick = 0;
}

void IngameScene::teardown()
{
    delete _world;
}

void IngameScene::update()
{
    auto& input = game().input();
    InputEvent e;

    while (input.poll(e))
    {
        if (e.type == InputEventType::KeyDown)
        {
            switch (e.key.scancode)
            {
            case Keyboard::Escape:
                save_world(*_world);
                game().set_scene<MainMenuScene>();
                return;
            default:
                update_ui(e, input.keyboard, *_world, _selection, _viewport);
                break;
            }
        }
        else if (e.type == InputEventType::Text)
        {
            switch (e.text.unicode)
            {
            case U'v':
                game().window().toggle_vsync();
                break;
            default:
                update_ui(e, input.keyboard, *_world, _selection, _viewport);
                break;
            }
        }
    }

    _world->map.tick();
    //update_ai(*_world, _update_tick);
    _ai.run();

    _update_tick++;
    _render_tick++;
}

void IngameScene::render(DrawBuffer& draw_buffer)
{
    _viewport = draw_buffer.size();
    draw_world(draw_buffer, *_world, game(), _render_tick);
    draw_selection(draw_buffer, *_world, _selection, _render_tick);
}
