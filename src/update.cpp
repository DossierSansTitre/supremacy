#include <game.h>
#include <rect3.h>

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

static void move_camera(Game& game, Vec3 motion)
{
    motion = clamp_motion(game, game.camera, motion);
    game.camera.x += motion.x;
    game.camera.y += motion.y;
    game.camera.z += motion.z;

    clamp_camera(game);
}

void move_selection(Game& game, Vec3 motion)
{
    motion = clamp_motion(game, game.cursor, motion);
    game.cursor.x += motion.x;
    game.cursor.y += motion.y;
    game.cursor.z += motion.z;
}

void handle_motion(Game& game)
{
    Vec3 motion;

    motion = keyboard_motion(game);
    if (game.ui_state == UiStateID::None)
    {
        move_camera(game, motion);
    }
    else
    {
        move_selection(game, motion);
    }
}

static void mine(Game& game, Rect3 rect)
{
    int x;
    int y;
    int z;

    for (int k = 0; k < rect.size.z; ++k)
    {
        for (int j = 0; j < rect.size.y; ++j)
        {
            for (int i = 0; i < rect.size.x; ++i)
            {
                x = rect.origin.x + i;
                y = rect.origin.y + j;
                z = rect.origin.z + k;

                game.map.set_action(x, y, z, MapAction::Mine);
            }
        }
    }
}

static void handle_ui_state_selection(Game& game)
{
    if (game.keyboard.key_pressed(SDLK_ESCAPE))
    {
        if (game.selected_first)
            game.selected_first = false;
        else
            game.ui_state = UiStateID::None;
    }
    else if (game.keyboard.key_pressed(SDLK_RETURN))
    {
        if (!game.selected_first)
        {
            game.selection[0] = game.cursor;
            game.selected_first = true;
        }
        else
        {
            game.selection[1] = game.cursor;
            game.ui_state = UiStateID::None;
            mine(game, rect_from_points(game.selection[0], game.selection[1]));
        }
    }
}

static void start_selection(Game& game)
{
    game.cursor = game.camera;
    game.cursor.x += (game.renderer.width() - 2) / 2;
    game.cursor.y += (game.renderer.height() - 2) / 2;
    game.ui_state = UiStateID::Selection;
    game.selected_first = false;
}

static void handle_ui_state_none(Game& game)
{
    if (game.keyboard.key_pressed(SDLK_m))
    {
        start_selection(game);
        return;
    }
    if (game.keyboard.key_pressed(SDLK_ESCAPE))
        game.running = false;
}

static void handle_ui_state(Game& game)
{
    switch (game.ui_state)
    {
        case UiStateID::None:
            handle_ui_state_none(game);
            break;
        case UiStateID::Selection:
            handle_ui_state_selection(game);
            break;
    }
}

void game_update(Game& game)
{
    game.fps_counter_update.update();
    game.tick_render++;
    if (game.keyboard.key_pressed(SDLK_v))
        toggle_vsync(game);
    handle_motion(game);
    handle_ui_state(game);
    game_ai(game);
}
