#include <ctime>
#include <game.h>

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
    game.camera_depth = 0;
    game.camera_x = 0;
    game.camera_y = 0;
    game.seed = time(nullptr);

    generate_map(game.map, game.seed);

    game.actors.add(ActorID::Dwarf, {0, 0, 0});
    game.actors.add(ActorID::Dwarf, {2, 2, 0});
    game.actors.add(ActorID::Goblin, {2, 0, 0});
    game.actors.add(ActorID::Goblin, {2, 1, 0});
 
    game.items.add(ItemID::Wood, {0, 1, 0}, 30);

    game_loop(game);

    return 0;
}
