#include <SDL2/SDL.h>
#include <task.h>
#include <math/rect.h>
#include <math/linear.h>
#include <update.h>
#include <world.h>
#include <selection.h>
#include <keyboard.h>

// TODO: Remove this
static uint16_t selection_task;
static bool vsync = true;

static void toggle_vsync()
{
    if (vsync)
    {
        SDL_GL_SetSwapInterval(0);
    }
    else
    {
        if (SDL_GL_SetSwapInterval(-1))
            SDL_GL_SetSwapInterval(1);
    }
    vsync = !vsync;
}

static Vector3i clamp_motion(const Map& map, Vector3i origin, Vector3i delta)
{
    if (origin.x + delta.x < 0)
        delta.x = -origin.x;
    else if (origin.x + delta.x >= map.width())
        delta.x = map.width() - 1 - origin.x;
    if (origin.y + delta.y < 0)
        delta.y = -origin.y;
    else if (origin.y + delta.y >= map.height())
        delta.y = map.height() - 1 - origin.y;
    if (origin.z + delta.z < 0)
        delta.z = -origin.z;
    else if (origin.z + delta.z >= map.depth())
        delta.z = map.depth() - 1 - origin.z;
    return delta;
}

static void clamp_camera(World& world, Vector2u viewport)
{
    int w;
    int h;

    const auto& map = world.map;
    auto& camera = world.camera;

    w = viewport.x - 2;
    h = viewport.y - 2;
    if (camera.x + w >= map.width())
        camera.x = map.width() - w;
    if (camera.y + h >= map.height())
        camera.y = map.height() - h;
}

static Vector3i keyboard_motion(Keyboard& keyboard)
{
    bool shift;
    int delta;
    int delta_z;
    Vector3i motion = {0, 0, 0};

    shift = keyboard.down(Keyboard::Shift);

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

    if (keyboard.repeated(Keyboard::Left))
        motion.x -= delta;
    if (keyboard.repeated(Keyboard::Right))
        motion.x += delta;
    if (keyboard.repeated(Keyboard::Up))
        motion.y -= delta;
    if (keyboard.repeated(Keyboard::Down))
        motion.y += delta;
    if (keyboard.key_repeated(Keyboard::Unknown))
        motion.z += delta_z;
    if (keyboard.key_repeated(Keyboard::Unknown))
        motion.z -= delta_z;
    return motion;
}

static void move_camera(World& world, Vector2u viewport, Vector3i motion)
{
    auto& camera = world.camera;

    motion = clamp_motion(world.map, camera, motion);
    camera += motion;
    clamp_camera(world, viewport);
}

void move_selection(World& world, Selection& selection, Vector3i motion)
{
    auto cursor = selection.cursor();
    motion = clamp_motion(world.map, cursor, motion);
    cursor += motion;
    selection.set_cursor(cursor);

    world.camera.z += motion.z;
}

void handle_motion(World& world, Keyboard& keyboard, Selection& selection, Vector2u viewport)
{
    Vector3i motion;

    motion = keyboard_motion(keyboard);
    if (selection.state() == Selection::State::Inactive)
        move_camera(world, viewport, motion);
    else
        move_selection(world, selection, motion);
}

static void set_action_rect(Map& map, Rect3i rect, uint16_t task)
{
    int x;
    int y;
    int z;

    for (int k = 0; k <= rect.size.z; ++k)
    {
        for (int j = 0; j <= rect.size.y; ++j)
        {
            for (int i = 0; i <= rect.size.x; ++i)
            {
                x = rect.origin.x + i;
                y = rect.origin.y + j;
                z = rect.origin.z + k;

                const Task& task_data = Task::from_id(task);

                for (uint16_t tile : task_data.match)
                {
                    if (map.tile_at(x, y, z) == TileID(tile))
                    {
                        map.set_task(x, y, z, task);
                        break;
                    }
                }
            }
        }
    }
}

static void handle_ui_state_selection(World& world, Keyboard& keyboard, Selection& selection)
{
    if (keyboard.key_pressed(SDLK_RETURN))
    {
        selection.advance();
        if (selection.state() == Selection::State::Inactive)
            set_action_rect(world.map, selection.selected(), selection_task);
    }
}

static void start_selection(World& world, Selection& selection, Vector2u viewport)
{
    Vector3i cursor;

    cursor = world.camera;
    cursor += (Vector3i(viewport, 0) - Vector3i(2, 2, 0)) / 2;
    selection.set_cursor(cursor);
    selection.advance();
}

static void handle_ui_state_none(World& world, Keyboard& keyboard, Selection& selection, Vector2u viewport)
{
    if (keyboard.key_pressed(SDLK_m))
    {
        selection_task = 1;
        start_selection(world, selection, viewport);
        return;
    }
    else if (keyboard.key_pressed(SDLK_n))
    {
        selection_task = 2;
        start_selection(world, selection, viewport);
        return;
    }
    else if (keyboard.key_pressed(SDLK_x))
    {
        selection_task = 3;
        start_selection(world, selection, viewport);
        return;
    }
}

void update_ui(World& world, Keyboard& keyboard, Selection& selection, Vector2u viewport)
{
    if (selection.state() == Selection::State::Inactive)
        handle_ui_state_none(world, keyboard, selection, viewport);
    else
        handle_ui_state_selection(world, keyboard, selection);
    handle_motion(world, keyboard, selection, viewport);
    if (keyboard.key_pressed(SDLK_v))
        toggle_vsync();
}
