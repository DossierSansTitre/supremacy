#include <tile.h>
#include <material.h>
#include <actor_data.h>
#include <item_data.h>
#include <math/rect.h>
#include <math/linear.h>
#include <world.h>
#include <engine/game.h>
#include <selection.h>

static void draw_rect_ingame(DrawBuffer& draw_buffer, World& world, Rect3i rect, int sym, Color color, Color color_bg)
{
    int view_w;
    int view_h;
    Vector3i camera = world.camera;

    if (camera.z < rect.origin.z || camera.z > rect.origin.z + rect.size.z)
        return;

    view_w = draw_buffer.width() - 2;
    view_h = draw_buffer.height() - 2;

    rect.origin.x -= camera.x;
    rect.origin.y -= camera.y;

    if (rect.origin.x >= view_w)
        return;
    if (rect.origin.y >= view_h)
        return;
    if (rect.origin.x + rect.size.x < 0)
        return;
    if (rect.origin.y + rect.size.y < 0)
        return;

    if (rect.origin.x < 0)
    {
        rect.size.x += rect.origin.x;
        rect.origin.x = 0;
    }
    if (rect.origin.y < 0)
    {
        rect.size.y += rect.origin.y;
        rect.origin.y = 0;
    }

    if (rect.origin.x + rect.size.x > view_w)
    {
        rect.size.x = view_w - rect.origin.x;
    }
    if (rect.origin.y + rect.size.y > view_h)
    {
        rect.size.y = view_h - rect.origin.y;
    }

    for (int j = 0; j <= rect.size.y; ++j)
    {
        for (int i = 0; i <= rect.size.x; ++i)
        {
            putchar_fast(draw_buffer, i + rect.origin.x + 1, j + rect.origin.y + 1, sym, color, color_bg);
        }
    }
}

void draw_selection(DrawBuffer& draw_buffer, World& world, Selection& selection, u32 render_tick)
{
    Vector3i cursor;
    Rect3i rect;
    char c;

    if (render_tick / 4 % 2)
        c = 'X';
    else
        c = ' ';
    if (selection.state() != Selection::State::Inactive)
    {
        cursor = selection.cursor();
        if (selection.state() == Selection::State::Second)
        {
            draw_rect_ingame(draw_buffer, world, selection.selected(), c, {200, 127, 127}, {180, 180, 180});
        }
        putchar(draw_buffer, cursor.x - world.camera.x + 1, cursor.y - world.camera.y + 1, c, {255, 127, 127}, {200, 200, 200});
    }
}

static void draw_bars(DrawBuffer& draw_buffer, World& world, Game& game)
{
    int fps_render;
    int fps_update;
    int view_w;
    int view_h;

    Vector3i camera = world.camera;

    view_w = draw_buffer.width();
    view_h = draw_buffer.height();

    fps_render = game.fps_counter_render().get();
    fps_update = game.fps_counter_update().get();

    for (int i = 0; i < view_w; ++i)
    {
        putchar(draw_buffer, i, 0, ' ', {0, 0, 0}, {255, 255, 255});
        putchar(draw_buffer, i, view_h - 1, ' ', {0, 0, 0}, {255, 255, 255});
    }
    for (int i = 1; i < view_h - 1; ++i)
    {
        putchar(draw_buffer, 0, i, ' ', {0, 0, 0}, {255, 255, 255});
        putchar(draw_buffer, view_w - 1, i, ' ', {0, 0, 0}, {255, 255, 255});
    }
    /* Top */
    printf(draw_buffer, 0, 0, "FPS: %d(%d)", {0, 0, 0}, {255, 255, 255}, fps_render, fps_update);
    print(draw_buffer, view_w / 2 - 5, 0, "SUPREMACY", {200, 10, 10}, {255, 255, 255});

    /* Bottom */
    printf(draw_buffer, 0, view_h - 1, "X:%-4d Y:%-4d Z:%-4d", {0, 0, 0}, {255, 255, 255}, camera.x, camera.y, camera.z);
}

static void draw_map_lines(DrawBuffer& draw_buffer, World& world, u32 render_tick, size_t base, size_t count)
{
    int view_w;

    int x;
    int y;
    int under;

    int limit = static_cast<int>(base + count);

    view_w = draw_buffer.width();

    Map& map = world.map;
    Vector3i camera = world.camera;

    for (int j = base; j < limit; ++j)
    {
        y = camera.y + j;
        for (int i = 0; i < view_w - 2; ++i)
        {
            x = camera.x + i;
            TileID tile_id;
            MaterialID material_id;
            MaterialID floor_material_id;
            uint16_t task;
            Map::Flash flash;
            uint16_t sym;
            Color color;
            Color color_bg;

            under = 0;
            map.at(x, y, camera.z, tile_id, material_id);
            floor_material_id = map.floor({x, y, camera.z});
            task = map.task_at(x, y, camera.z);
            flash = map.flash({x, y, camera.z});

            while (!material_id && !floor_material_id)
            {
                if (under >= 3)
                    break;
                under++;
                map.at(x, y, camera.z - under, tile_id, material_id);
                floor_material_id = map.floor({x, y, camera.z - under});
                task = map.task_at(x, y, camera.z - under);
                flash = map.flash({x, y, camera.z - under});
            }

            if ((!material_id && !floor_material_id) || ((!map.visible(x, y, camera.z) && task == 0) && 1))
                continue;

            if (flash == Map::Flash::Action)
            {
                sym = "\\|/-"[render_tick % 4];
                color = {0, 0, 0};
                color_bg = {255, 255, 0};
            }
            else if (task)
            {
                sym = '!';
                color = {255, 255, 255};
                color_bg = {255, 0, 0};
            }
            else
            {
                /* There is no material, a.k.a. we have to render some floor */
                if (!material_id)
                {
                    const Material& material = Material::from_id(floor_material_id);
                    sym = under ? ' ' : 130;
                    color = material.color;
                    color_bg = material.color_bg;
                }
                else
                {
                    const Tile& tile = Tile::from_id(tile_id);
                    const Material& material = Material::from_id(material_id);
                    if (under && tile.dim_sym)
                        sym = tile.dim_sym;
                    else
                        sym = tile.sym;
                    color = material.color;
                    color_bg = material.color_bg;
                }
            }

            if (flash == Map::Flash::Pending)
            {
                if ((render_tick / 2) % 2)
                {
                    color *= 0.75f;
                    color_bg *= 0.75f;
                }
                else
                {
                    color *= 0.5f;
                    color_bg *= 0.5f;
                }
            }

            while (under--)
            {
                color *= 0.5f;
                color_bg *= 0.5f;
            }
            putchar_fast(draw_buffer, i + 1, j + 1, sym, color, color_bg);
        }
    }
}

static void draw_map(DrawBuffer& draw_buffer, World& world, Game& game, u32 render_tick)
{
    static const size_t lines_per_job = 8;

    auto& thread_pool = game.thread_pool();

    int task;
    int view_h;
    size_t job_count;

    view_h = draw_buffer.height() - 2;

    job_count = ceil((float)view_h / lines_per_job);

    task = thread_pool.task_create();
    for (size_t i = 0; i < job_count - 1; ++i)
    {
        thread_pool.task_perform(task, std::bind(draw_map_lines, std::ref(draw_buffer), std::ref(world), render_tick, i * lines_per_job, lines_per_job));
    }
    thread_pool.task_perform(task, std::bind(draw_map_lines, std::ref(draw_buffer), std::ref(world), render_tick, (job_count - 1) * lines_per_job, view_h - (job_count - 1) * lines_per_job));
    thread_pool.task_wait(task);
}

static void draw_actors(DrawBuffer& draw_buffer, World& world, u32 render_tick, int delta_z)
{
    int count;
    int x;
    int y;
    int z;
    int view_w;
    int view_h;
    uint16_t sym;
    Color color;
    Color color_bg;
    int anim;
    static const char anim_str[] = "-\\|/";

    view_w = draw_buffer.width();
    view_h = draw_buffer.height();
    Vector3i camera = world.camera;
    auto& actors = world.actors;

    z = camera.z - delta_z;
    count = actors.count();
    color_bg = {0, 0, 0};
    for (int i = 0; i < count; ++i)
    {
        ActorID actor_id;
        Vector3i pos;

        actor_id = actors.actor_id(i);
        if (actor_id == ActorID::None)
            continue;
        pos = actors.pos(i);
        if (pos.z != z)
            continue;

        x = pos.x - camera.x;
        y = pos.y - camera.y;

        if (x < 0 || x >= view_w - 2 || y < 0 || y >= view_h - 2)
            continue;

        const ActorData& actor_data = ActorData::from_id(actor_id);
        sym = actor_data.sym;
        color = actor_data.color;
        /* Useless animation to test the system */
        anim = render_tick % 40;
        if (anim < 4)
        {
            color = {255, 255, 255};
            sym = anim_str[anim];
        }
        for (int j = 0; j < delta_z; ++j)
            color *= 0.5;
        putchar(draw_buffer, x + 1, y + 1, sym, color, color_bg);
    }
}

static void draw_actors(DrawBuffer& draw_buffer, World& world, u32 render_tick)
{
    for (int i = 0; i < 4; ++i)
        draw_actors(draw_buffer, world, render_tick, 3 - i);
}

static void draw_items(DrawBuffer& draw_buffer, World& world)
{
    int count;
    int x;
    int y;
    int view_w;
    int view_h;
    uint16_t sym;
    Color color;
    Color color_bg;

    view_w = draw_buffer.width();
    view_h = draw_buffer.height();

    color_bg = {0, 0, 0};

    auto& items = world.items;
    Vector3i camera = world.camera;

    count = items.count();
    for (int i = 0; i < count; ++i)
    {
        ItemID item_id;
        Vector3i pos;

        item_id = items.item_id(i);
        if (!item_id)
            continue;
        pos = items.pos(i);
        if (pos.z != camera.z)
            continue;

        x = pos.x - camera.x;
        y = pos.y - camera.y;

        if (x < 0 || x >= view_w - 2 || y < 0 || y >= view_h - 2)
            continue;

        const ItemData& item_data = ItemData::from_id(item_id);
        sym = item_data.sym;
        color = item_data.color;
        putchar(draw_buffer, x + 1, y + 1, sym, color, color_bg);
    }
}

void draw_world(DrawBuffer& buffer, World& world, Game& game, u32 render_tick)
{
    draw_map(buffer, world, game, render_tick);
    draw_items(buffer, world);
    draw_actors(buffer, world, render_tick);
    draw_bars(buffer, world, game);
}
