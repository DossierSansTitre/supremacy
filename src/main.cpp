#include <ctime>
#include <game.h>
#include <archive.h>
#include <thread_pool.h>
#include <tile.h>
#include <material.h>
#include <item_data.h>
#include <biome.h>
#include <task.h>
#include <util/file_path.h>
#include <ctime>

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

static void generate_dwarfs(Game& game)
{
    int w;
    int h;
    int x;
    int y;
    int z;
    int spawn_count;

    w = game.map.width();
    h = game.map.height();

    spawn_count = 0;

    while (spawn_count < 7)
    {
        x = (w / 2) + (rand() % 16) - 8;
        y = (h / 2) + (rand() % 16) - 8;
        z = find_suitable_height(game.map, x, y);
        game.actors.add(ActorID::Dwarf, {x, y, z});
        spawn_count++;
    }
    game.camera.x = x - game.draw_buffer.width() / 2;
    game.camera.y = y - game.draw_buffer.height() / 2;
    game.camera.z = z;
}

void load_game_data()
{
    Archive archive;

    archive.open(data_path("/supremacy.bin"));
    Tile::load(archive);
    Material::load(archive);
    ItemData::load(archive);
    Biome::load(archive);
    Task::load(archive);
}

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    Game game;

    load_game_data();

    srand(time(nullptr));

    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    game.window.init();
    game.renderer = new Renderer(game.thread_pool);
    game.draw_buffer.resize(game.window.width() / 8, game.window.height() / 12);

    // KLUDGE
    game.seed = static_cast<uint32_t>(time(nullptr));
    game.tick = 0;
    game.tick_render = 0;

    generate_map(game.map, game.seed);
    generate_dwarfs(game);

    game.vsync = 1;

    game_loop(game);

    SDL_Quit();
    return 0;
}
