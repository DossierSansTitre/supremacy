#include <game.h>

static void toggle_vsync(Game& game)
{
    if (game.vsync)
    {
        SDL_GL_SetSwapInterval(0);
    }
    else
    {
        if (SDL_GL_SetSwapInterval(-1))
            SDL_GL_SetSwapInterval(1);
    }
    game.vsync = !game.vsync;
}

static void handle_camera(Game& game)
{
    bool shift;
    int delta;
    int delta_z;

    shift = (game.keyboard.down(SDL_SCANCODE_LSHIFT) || game.keyboard.down(SDL_SCANCODE_RSHIFT));

    if (shift)
    {
        delta = 10;
        delta_z = 5;
    }
    else
    {
        delta = 1;
        delta_z = 1;
    }

    if (game.keyboard.repeated(SDL_SCANCODE_LEFT))
        game.camera.x -= delta;
    if (game.keyboard.repeated(SDL_SCANCODE_RIGHT))
        game.camera.x += delta;
    if (game.keyboard.repeated(SDL_SCANCODE_UP))
        game.camera.y -= delta;
    if (game.keyboard.repeated(SDL_SCANCODE_DOWN))
        game.camera.y += delta;
    if (game.keyboard.key_repeated(SDLK_q))
        game.camera.z += delta_z;
    if (game.keyboard.key_repeated(SDLK_e))
        game.camera.z -= delta_z;
    if (game.camera.z < 0)
        game.camera.z = 0;
    if (game.camera.z >= game.map.depth())
        game.camera.z = game.map.depth() - 1;
}

void game_update(Game& game)
{
    game.fps_counter_update.update();
    if (game.keyboard.pressed(SDL_SCANCODE_ESCAPE))
        game.running = false;
    if (game.keyboard.key_pressed(SDLK_v))
        toggle_vsync(game);
    handle_camera(game);
    game_ai(game);
}
