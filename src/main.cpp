#include <cstdlib>
#include <ctime>
#include <engine/game.h>
#include <scene/main_menu_scene.h>
#include <init.h>
#include <cli_options.h>

int main(int argc, char** argv)
{
    Game game;
    int status;

    status = 0;

    CliOptions::instance().parse(argc, argv);
    srand((unsigned int)time(nullptr));

    game.select_renderer();
    if (init_game_data())
    {
        game.set_scene<MainMenuScene>();
        game.loop();
    }
    else
        status = 1;
    return status;
}
