#include <ctime>
#include <game.h>

static int find_suitable_height(const Map& map, int x, int y)
{
    int depth = map.depth();

    for (int k = depth - 1; k >= 0; --k)
    {
        if (map.tile_at(x, y, k) != TileID::None)
            return k;
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
    game.camera_x = x - game.renderer.width() / 2;
    game.camera_y = y - game.renderer.height() / 2;
    game.camera_depth = z;
}

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    Game game;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    game.window.init();
    game.renderer.init(game.window.width(), game.window.height());

    // KLUDGE
    game.seed = static_cast<uint32_t>(time(nullptr));

    generate_map(game.map, game.seed);
    generate_dwarfs(game);

    game.items.add(ItemID::Wood, {0, 1, 0}, 30);
    game.vsync = 1;

    game_loop(game);

    return 0;
}
