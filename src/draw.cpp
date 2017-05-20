#include <game.h>
#include <tile.h>
#include <material.h>
#include <actor_data.h>
#include <item_data.h>
#include <rect3.h>

static void draw_rect_ingame(Game& game, Rect3 rect, int sym, Color color, Color color_bg)
{
    int view_w;
    int view_h;

    if (game.camera.z < rect.origin.z || game.camera.z >= rect.origin.z + rect.size.z)
        return;

    view_w = game.renderer.width() - 2;
    view_h = game.renderer.height() - 2;

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

    for (int j = 0; j < rect.size.y; ++j)
    {
        for (int i = 0; i < rect.size.x; ++i)
        {
            game.renderer.putchar_fast(i + rect.origin.x + 1, j + rect.origin.y + 1, sym, color, color_bg);
        }
    }
}

static void draw_ui_state(Game& game)
{
    Vec3 cursor;
    Rect3 rect;
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
        game.renderer.putchar(cursor.x - game.camera.x + 1, cursor.y - game.camera.y + 1, c, {255, 127, 127}, {200, 200, 200});
    }
}

static void draw_bars(Game& game)
{
    int fps_render;
    int fps_update;
    int view_w;
    int view_h;

    view_w = game.renderer.width();
    view_h = game.renderer.height();

    fps_render = game.fps_counter_render.get();
    fps_update = game.fps_counter_update.get();

    for (int i = 0; i < view_w; ++i)
    {
        game.renderer.putchar(i, 0, ' ', {0, 0, 0}, {255, 255, 255});
        game.renderer.putchar(i, view_h - 1, ' ', {0, 0, 0}, {255, 255, 255});
    }
    for (int i = 1; i < view_h - 1; ++i)
    {
        game.renderer.putchar(0, i, ' ', {0, 0, 0}, {255, 255, 255});
        game.renderer.putchar(view_w - 1, i, ' ', {0, 0, 0}, {255, 255, 255});
    }
    /* Top */
    game.renderer.printf(0, 0, "FPS: %d(%d)", {0, 0, 0}, {255, 255, 255}, fps_render, fps_update);
    game.renderer.print(view_w / 2 - 5, 0, "SUPREMACY", {200, 10, 10}, {255, 255, 255});

    /* Bottom */
    game.renderer.printf(0, view_h - 1, "Z: %-3d", {0, 0, 0}, {255, 255, 255}, game.camera.z);
}

static void draw_map_lines(Game& game, size_t base, size_t count)
{
    int view_w;

    int x;
    int y;
    int under;

    int limit = static_cast<int>(base + count);

    view_w = game.renderer.width();

    for (int j = base; j < limit; ++j)
    {
        y = game.camera.y + j;
        for (int i = 0; i < view_w - 2; ++i)
        {
            x = game.camera.x + i;
            TileID tile_id;
            MaterialID material_id;
            uint16_t sym;
            Color color;
            Color color_bg;

            under = 0;
            game.map.at(x, y, game.camera.z, tile_id, material_id);

            while (tile_id == TileID::None)
            {
                if (under >= 3)
                    break;
                under++;
                game.map.at(x, y, game.camera.z - under, tile_id, material_id);
            }

            if (tile_id == TileID::None)
                continue;

            const Tile& tile = Tile::from_id(tile_id);
            const Material& material = Material::from_id(material_id);

            sym = tile.sym;
            color = material.color;
            color_bg = material.color_bg;

            if (under)
            {
                if (tile.dim_sym != 0)
                    sym = tile.dim_sym;
                while (under--)
                {
                    color *= 0.5f;
                    color_bg *= 0.5f;
                }
            }
            game.renderer.putchar_fast(i + 1, j + 1, sym, color, color_bg);
        }
    }
}

static void draw_map(Game& game)
{
    static const size_t lines_per_job = 8;

    ThreadPool& thread_pool(game.thread_pool);

    int task;
    int view_h;
    size_t job_count;

    view_h = game.renderer.height() - 2;

    job_count = ceil((float)view_h / lines_per_job);

    task = thread_pool.task_create();
    for (size_t i = 0; i < job_count - 1; ++i)
    {
        thread_pool.task_perform(task, std::bind(draw_map_lines, std::ref(game), i * lines_per_job, lines_per_job));
    }
    thread_pool.task_perform(task, std::bind(draw_map_lines, std::ref(game), (job_count - 1) * lines_per_job, view_h - (job_count - 1) * lines_per_job));
    thread_pool.task_wait(task);
}

static void draw_actors(Game& game)
{
    int count;
    int x;
    int y;
    int view_w;
    int view_h;
    uint16_t sym;
    Color color;
    Color color_bg;
    int anim;
    static const char anim_str[] = "-\\|/";

    view_w = game.renderer.width();
    view_h = game.renderer.height();

    count = game.actors.count();
    color_bg = {0, 0, 0};
    for (int i = 0; i < count; ++i)
    {
        ActorID actor_id;
        Vec3 pos;

        actor_id = game.actors.actor_id(i);
        if (actor_id == ActorID::None)
            continue;
        pos = game.actors.pos(i);
        if (pos.z != game.camera.z)
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
        game.renderer.putchar(x + 1, y + 1, sym, color, color_bg);
    }
}

static void draw_items(Game& game)
{
    int count;
    int x;
    int y;
    int view_w;
    int view_h;
    uint16_t sym;
    Color color;
    Color color_bg;

    view_w = game.renderer.width();
    view_h = game.renderer.height();

    color_bg = {0, 0, 0};

    count = game.items.count();
    for (int i = 0; i < count; ++i)
    {
        ItemID item_id;
        Vec3 pos;

        item_id = game.items.item_id(i);
        if (item_id == ItemID::None)
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
        game.renderer.putchar(x + 1, y + 1, sym, color, color_bg);
    }
}


void game_draw(Game& game)
{
    game.fps_counter_render.update();
    game.renderer.clear();
    draw_map(game);
    draw_items(game);
    draw_actors(game);
    draw_ui_state(game);
    draw_bars(game);
    game.renderer.render();
    game.window.swap();
}
