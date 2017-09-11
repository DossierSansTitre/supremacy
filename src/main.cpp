#include <cstdlib>
#include <ctime>
#include <engine/game.h>
#include <window.h>
#include <scene/main_menu_scene.h>
#include <init.h>
#include <noise.h>
#include <log.h>

static void img_dump(int size)
{
    FILE* img;
    int value;
    int seed;

    seed = rand();

    img = fopen("img.pgm", "w");
    fprintf(img, "P2\n%d %d\n255\n", size, size);
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            float f = ((noise_fractal_octave_2d(seed, x, y, 1.6, 9) + 0.707f) / 1.414f) * 2;
            value = f * 256;
            fprintf(img, "%d%s", value, (x == (size - 1)) ? "\n" : " ");
        }
    }
    fclose(img);
    exit(0);
}

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
    int status;
    Window* window;

    status = 0;

    srand(time(nullptr));
    //img_dump(1024);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = create_window();
    if (init_game_data())
        launch_game(*window);
    else
        status = 1;

    delete window;

    SDL_Quit();
    return status;
}
