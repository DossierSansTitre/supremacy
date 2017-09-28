#include <cstdlib>
#include <ctime>
#include <engine/game.h>
#include <scene/main_menu_scene.h>
#include <init.h>

int main()
{
    Game game;
    int status;

    status = 0;

    srand(time(nullptr));

    SDL_Init(SDL_INIT_VIDEO);
    game.select_renderer();
    if (init_game_data())
    {
        game.set_scene<MainMenuScene>();
        game.loop();
    }
    else
        status = 1;
    SDL_Quit();
    return status;
}
