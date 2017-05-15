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

static Vec3 clamp_motion(Game& game, Vec3 origin, Vec3 delta)
{
    if (origin.x + delta.x < 0)
        delta.x = -origin.x;
    else if (origin.x + delta.x >= game.map.width())
        delta.x = game.map.width() - 1 - origin.x;
    if (origin.y + delta.y < 0)
        delta.y = -origin.y;
    else if (origin.y + delta.y >= game.map.height())
        delta.y = game.map.height() - 1 - origin.y;
    if (origin.z + delta.z < 0)
        delta.z = -origin.z;
    else if (origin.z + delta.z >= game.map.depth())
        delta.z = game.map.depth() - 1 - origin.z;
    return delta;
}

static void clamp_camera(Game& game)
{
    int w;
    int h;

    w = game.renderer.width() - 2;
    h = game.renderer.height() - 2;
    if (game.camera.x + w >= game.map.width())
        game.camera.x = game.map.width() - w;
    if (game.camera.y + h >= game.map.height())
        game.camera.y = game.map.height() - h;
}

static Vec3 keyboard_motion(Game& game)
{
    bool shift;
    int delta;
    int delta_z;
    Vec3 motion = {0, 0, 0};

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
        motion.x -= delta;
    if (game.keyboard.repeated(SDL_SCANCODE_RIGHT))
        motion.x += delta;
    if (game.keyboard.repeated(SDL_SCANCODE_UP))
        motion.y -= delta;
    if (game.keyboard.repeated(SDL_SCANCODE_DOWN))
        motion.y += delta;
    if (game.keyboard.key_repeated(SDLK_q))
        motion.z += delta_z;
    if (game.keyboard.key_repeated(SDLK_e))
        motion.z -= delta_z;
    return motion;
}

static void handle_camera(Game& game)
{
    Vec3 motion;

    motion = clamp_motion(game, game.camera, keyboard_motion(game));
    game.camera.x += motion.x;
    game.camera.y += motion.y;
    game.camera.z += motion.z;

    clamp_camera(game);
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
