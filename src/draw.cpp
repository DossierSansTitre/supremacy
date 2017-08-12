#include <game_state.h>
#include <tile.h>
#include <material.h>
#include <actor_data.h>
#include <item_data.h>
#include <math/rect.h>
#include <math/linear.h>

static void draw_rect_ingame(GameState& game, Rect3i rect, int sym, Color color, Color color_bg)
{
    int view_w;
    int view_h;

    if (game.camera.z < rect.origin.z || game.camera.z > rect.origin.z + rect.size.z)
        return;

    view_w = game.draw_buffer.width() - 2;
    view_h = game.draw_buffer.height() - 2;

    rect.origin.x -= game.camera.x;
    rect.origin.y -= game.camera.y;

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
            putchar_fast(game.draw_buffer, i + rect.origin.x + 1, j + rect.origin.y + 1, sym, color, color_bg);
        }
    }
}

static void draw_ui_state(GameState& game)
{
    Vector3i cursor;
    Rect3i rect;
    char c;

    if (game.tick_render / 4 % 2)
        c = 'X';
    else
        c = ' ';
    if (game.ui_state == UiStateID::Selection)
    {
        cursor = game.cursor;
        if (game.selected_first)
        {
            rect = rect_from_points(cursor, game.selection[0]);
            draw_rect_ingame(game, rect, c, {200, 127, 127}, {180, 180, 180});
        }
        putchar(game.draw_buffer, cursor.x - game.camera.x + 1, cursor.y - game.camera.y + 1, c, {255, 127, 127}, {200, 200, 200});
    }
}

static void draw_bars(GameState& game)
{
    int fps_render;
    int fps_update;
    int view_w;
    int view_h;

    view_w = game.draw_buffer.width();
    view_h = game.draw_buffer.height();

    fps_render = game.fps_counter_render.get();
    fps_update = game.fps_counter_update.get();

    for (int i = 0; i < view_w; ++i)
    {
        putchar(game.draw_buffer, i, 0, ' ', {0, 0, 0}, {255, 255, 255});
        putchar(game.draw_buffer, i, view_h - 1, ' ', {0, 0, 0}, {255, 255, 255});
    }
    for (int i = 1; i < view_h - 1; ++i)
    {
        putchar(game.draw_buffer, 0, i, ' ', {0, 0, 0}, {255, 255, 255});
        putchar(game.draw_buffer, view_w - 1, i, ' ', {0, 0, 0}, {255, 255, 255});
    }
    /* Top */
    printf(game.draw_buffer, 0, 0, "FPS: %d(%d)", {0, 0, 0}, {255, 255, 255}, fps_render, fps_update);
    print(game.draw_buffer, view_w / 2 - 5, 0, "SUPREMACY", {200, 10, 10}, {255, 255, 255});

    /* Bottom */
    printf(game.draw_buffer, 0, view_h - 1, "Z: %-3d", {0, 0, 0}, {255, 255, 255}, game.camera.z);
}

static void draw_map_lines(GameState& game, size_t base, size_t count)
{
    int view_w;

    int x;
    int y;
    int under;

    int limit = static_cast<int>(base + count);

    view_w = game.draw_buffer.width();

    for (int j = base; j < limit; ++j)
    {
        y = game.camera.y + j;
        for (int i = 0; i < view_w - 2; ++i)
        {
            x = game.camera.x + i;
            TileID tile_id;
            MaterialID material_id;
            MaterialID floor_material_id;
            uint16_t task;
            Map::Flash flash;
            uint16_t sym;
            Color color;
            Color color_bg;

            under = 0;
            game.map.at(x, y, game.camera.z, tile_id, material_id);
            floor_material_id = game.map.floor({x, y, game.camera.z});
            task = game.map.task_at(x, y, game.camera.z);
            flash = game.map.flash({x, y, game.camera.z});

            while (!material_id && !floor_material_id)
            {
                if (under >= 3)
                    break;
                under++;
                game.map.at(x, y, game.camera.z - under, tile_id, material_id);
                floor_material_id = game.map.floor({x, y, game.camera.z - under});
                task = game.map.task_at(x, y, game.camera.z - under);
                flash = game.map.flash({x, y, game.camera.z - under});
            }

            if ((!material_id && !floor_material_id) || ((!game.map.visible(x, y, game.camera.z) && task == 0) && 1))
                continue;

            if (flash == Map::Flash::Action)
            {
                sym = "\\|/-"[game.tick_render % 4];
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
                if ((game.tick_render / 2) % 2)
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
            putchar_fast(game.draw_buffer, i + 1, j + 1, sym, color, color_bg);
        }
    }
}

static void draw_map(GameState& game)
{
    static const size_t lines_per_job = 8;

    ThreadPool& thread_pool(game.thread_pool);

    int task;
    int view_h;
    size_t job_count;

    view_h = game.draw_buffer.height() - 2;

    job_count = ceil((float)view_h / lines_per_job);

    task = thread_pool.task_create();
    for (size_t i = 0; i < job_count - 1; ++i)
    {
        thread_pool.task_perform(task, std::bind(draw_map_lines, std::ref(game), i * lines_per_job, lines_per_job));
    }
    thread_pool.task_perform(task, std::bind(draw_map_lines, std::ref(game), (job_count - 1) * lines_per_job, view_h - (job_count - 1) * lines_per_job));
    thread_pool.task_wait(task);
}

static void draw_actors(GameState& game, int delta_z)
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

    view_w = game.draw_buffer.width();
    view_h = game.draw_buffer.height();

    z = game.camera.z - delta_z;
    count = game.actors.count();
    color_bg = {0, 0, 0};
    for (int i = 0; i < count; ++i)
    {
        ActorID actor_id;
        Vector3i pos;

        actor_id = game.actors.actor_id(i);
        if (actor_id == ActorID::None)
            continue;
        pos = game.actors.pos(i);
        if (pos.z != z)
            continue;

        x = pos.x - game.camera.x;
        y = pos.y - game.camera.y;

        if (x < 0 || x >= view_w - 2 || y < 0 || y >= view_h - 2)
            continue;

        const ActorData& actor_data = ActorData::from_id(actor_id);
        sym = actor_data.sym;
        color = actor_data.color;
        /* Useless animation to test the system */
        anim = game.tick_render % 40;
        if (anim < 4)
        {
            color = {255, 255, 255};
            sym = anim_str[anim];
        }
        for (int j = 0; j < delta_z; ++j)
            color *= 0.5;
        putchar(game.draw_buffer, x + 1, y + 1, sym, color, color_bg);
    }
}

static void draw_actors(GameState& game)
{
    for (int i = 0; i < 4; ++i)
        draw_actors(game, 3 - i);
}

static void draw_items(GameState& game)
{
    int count;
    int x;
    int y;
    int view_w;
    int view_h;
    uint16_t sym;
    Color color;
    Color color_bg;

    view_w = game.draw_buffer.width();
    view_h = game.draw_buffer.height();

    color_bg = {0, 0, 0};

    count = game.items.count();
    for (int i = 0; i < count; ++i)
    {
        ItemID item_id;
        Vector3i pos;

        item_id = game.items.item_id(i);
        if (!item_id)
            continue;
        pos = game.items.pos(i);
        if (pos.z != game.camera.z)
            continue;

        x = pos.x - game.camera.x;
        y = pos.y - game.camera.y;

        if (x < 0 || x >= view_w - 2 || y < 0 || y >= view_h - 2)
            continue;

        const ItemData& item_data = ItemData::from_id(item_id);
        sym = item_data.sym;
        color = item_data.color;
        putchar(game.draw_buffer, x + 1, y + 1, sym, color, color_bg);
    }
}

void game_draw(GameState& game)
{
    game.fps_counter_render.update();
    game.draw_buffer.clear();
    draw_map(game);
    draw_items(game);
    draw_actors(game);
    draw_ui_state(game);
    draw_bars(game);
    game.renderer->render(game.draw_buffer);
    game.window.swap();
}
