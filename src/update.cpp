#include <SDL2/SDL.h>
#include <game/resource/task_terrain_data.h>
#include <math/rect.h>
#include <math/linear.h>
#include <update.h>
#include <world.h>
#include <selection.h>
#include <keyboard.h>

// TODO: Remove this
static uint16_t selection_task;

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

static bool handle_motion(InputEvent event, Keyboard& keyboard, Vector3i& motion)
{
    if (event.type == InputEventType::KeyDown)
    {
        bool shift = keyboard.down(Keyboard::Shift);
        int speed = shift ? 10 : 1;

        switch (event.key.scancode)
        {
            case Keyboard::Left:
                motion = Vector3i(-speed, 0, 0);
                return true;
            case Keyboard::Right:
                motion = Vector3i(speed, 0, 0);
                return true;
            case Keyboard::Up:
                motion = Vector3i(0, -speed, 0);
                return true;
            case Keyboard::Down:
                motion = Vector3i(0, speed, 0);
                return true;
        }
    }
    else if (event.type == InputEventType::Text)
    {
        switch (event.text.unicode)
        {
            case U'>':
                motion = Vector3i(0, 0, -1);
                return true;
            case U'<':
                motion = Vector3i(0, 0, 1);
                return true;
        }
    }
    return false;
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

                const auto& task_data = TaskTerrainData::from_id(task);

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

static void handle_ui_state_selection(InputEvent event, Keyboard& keyboard, World& world, Selection& selection, Vector2u viewport)
{
    Vector3i motion;

    (void)viewport;

    if (event.type == InputEventType::KeyDown)
    {
        switch (event.key.scancode)
        {
            case Keyboard::Enter:
                selection.advance();
                if (selection.state() == Selection::State::Inactive)
                    set_action_rect(world.map, selection.selected(), selection_task);
                return;

        }
    }
    if (handle_motion(event, keyboard, motion))
    {
        auto cursor = selection.cursor();
        motion = clamp_motion(world.map, cursor, motion);
        cursor += motion;
        selection.set_cursor(cursor);
        world.camera.z += motion.z;
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

static void handle_ui_state_none(InputEvent event, Keyboard& keyboard, World& world, Selection& selection, Vector2u viewport)
{
    Vector3i motion;

    if (event.type == InputEventType::Text)
    {
        switch (event.text.unicode)
        {
            case U'm':
                selection_task = 1;
                start_selection(world, selection, viewport);
                return;
            case U'n':
                selection_task = 2;
                start_selection(world, selection, viewport);
                return;
            case U'x':
                selection_task = 3;
                start_selection(world, selection, viewport);
                return;
            case U't':
                selection_task = 4;
                start_selection(world, selection, viewport);
                return;
        }
    }
    if (handle_motion(event, keyboard, motion))
    {
        auto& camera = world.camera;

        motion = clamp_motion(world.map, camera, motion);
        camera += motion;
        clamp_camera(world, viewport);
    }
}

void update_ui(InputEvent event, Keyboard& keyboard, World& world, Selection& selection, Vector2u viewport)
{
    if (selection.state() == Selection::State::Inactive)
        handle_ui_state_none(event, keyboard, world, selection, viewport);
    else
        handle_ui_state_selection(event, keyboard, world, selection, viewport);
}
