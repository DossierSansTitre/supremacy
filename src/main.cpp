#include <cstdlib>
#include <ctime>
#include <engine/game.h>
#include <window.h>
#include <scene/main_menu_scene.h>
#include <init.h>

static Window* create_window()
{
    Window* window;

    window = new Window;
    window->init();
    return window;
}

static void launch_game(Window& window)
{
    Game game(window);

    game.set_scene<MainMenuScene>();
    game.loop();
}

int main()
{
    Window* window;

    srand(time(nullptr));

    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = create_window();
    init_game_data();
    launch_game(*window);

    delete window;

    SDL_Quit();
    return 0;
}
