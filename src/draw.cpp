#include <game.h>
#include <tile.h>
#include <material.h>
#include <actor_data.h>
#include <item_data.h>

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
    /* Top */
    game.renderer.printf(0, 0, "FPS: %d(%d)", {0, 0, 0}, {255, 255, 255}, fps_render, fps_update);
    game.renderer.print(view_w / 2 - 5, 0, "SUPREMACY", {200, 10, 10}, {255, 255, 255});

    /* Bottom */
    game.renderer.printf(0, view_h - 1, "Z: %-3d", {0, 0, 0}, {255, 255, 255}, game.camera.z);
}

static void draw_map(Game& game)
{
    int view_w;
    int view_h;

    int x;
    int y;
    int under;

    view_w = game.renderer.width();
    view_h = game.renderer.height();

    for (int j = 0; j < (view_h - 2); ++j)
    {
        y = game.camera.y + j;
        for (int i = 0; i < view_w; ++i)
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
            game.renderer.putchar_fast(i, j + 1, sym, color, color_bg);
        }
    }
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

        if (x < 0 || x >= view_w || y < 0 || y >= (view_h - 2))
            continue;

        const ActorData& actor_data = ActorData::from_id(actor_id);
        sym = actor_data.sym;
        color = actor_data.color;
        game.renderer.putchar(x, y + 1, sym, color, color_bg);
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

        if (x < 0 || x >= view_w || y < 0 || y >= (view_h - 2))
            continue;

        const ItemData& item_data = ItemData::from_id(item_id);
        sym = item_data.sym;
        color = item_data.color;
        game.renderer.putchar(x, y + 1, sym, color, color_bg);
    }
}


void game_draw(Game& game)
{
    game.fps_counter_render.update();
    game.renderer.clear();
    draw_bars(game);
    draw_map(game);
    draw_items(game);
    draw_actors(game);
    game.renderer.render();
    game.window.swap();
}
