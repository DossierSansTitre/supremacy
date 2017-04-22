#include <wish.h>
#include <game.h>
#include <tile.h>
#include <material.h>
#include <actor_data.h>
#include <item_data.h>

static void render_top_bar(Game& game)
{
    int fps_render;
    int fps_update;

    wish_attr attr;
    wish_size size;

    wish_attr_init(&attr);
    wish_get_view_size(game.screen.screen, &size);

    fps_render = game.fps_counter_render.get();
    fps_update = game.fps_counter_update.get();

    wish_color(&attr, 15);
    wish_move(game.screen.top_bar, 0, 0);
    wish_printf(game.screen.top_bar, "FPS: %d(%d)", attr, fps_render, fps_update);

    wish_color(&attr, 9);
    wish_move(game.screen.top_bar, size.x / 2 - 4, 0);
    wish_puts(game.screen.top_bar, "SUPREMACY", attr);
}

static void render_map(Game& game)
{
    int x;
    int y;

    bool under;
    wish_attr attr;
    wish_size size;

    wish_attr_init(&attr);
    wish_get_view_size(game.screen.screen, &size);

    for (int j = 0; j < size.y; ++j)
    {
        y = game.camera_y + j;
        wish_move(game.screen.screen, 0, j);
        for (int i = 0; i < size.x; ++i)
        {
            x = game.camera_x + i;
            if (1 || game.map.visible(x, y, game.camera_depth))
            {
                TileID tile_id;
                MaterialID material_id;
                int sym;

                under = false;
                game.map.at(x, y, game.camera_depth, tile_id, material_id);

                if (tile_id == TileID::None)
                {
                    under = true;
                    game.map.at(x, y, game.camera_depth - 1, tile_id, material_id);
                }

                const Tile& tile = Tile::from_id(tile_id);
                const Material& material = Material::from_id(material_id);

                sym = tile.sym;
                if (under)
                {
                    if (tile.dim_sym != 0)
                        sym = tile.dim_sym;
                    wish_color(&attr, material.dim_color);
                    wish_bcolor(&attr, material.dim_bgcolor);
                }
                else
                {
                    wish_color(&attr, material.color);
                    wish_bcolor(&attr, material.bgcolor);
                }
                wish_putchar(game.screen.screen, sym, attr);
            }
            else
            {
                wish_color(&attr, 0);
                wish_bcolor(&attr, 0);
                wish_putchar(game.screen.screen, ' ', attr);
            }
        }
    }
}

static void render_actors(Game& game)
{
    wish_attr attr;
    wish_size size;
    int count;
    int x;
    int y;

    wish_attr_init(&attr);
    wish_get_view_size(game.screen.screen, &size);
    count = game.actors.count();
    for (int i = 0; i < count; ++i)
    {
        ActorID actor_id;
        Vec3 pos;

        actor_id = game.actors.actor_id(i);
        if (actor_id == ActorID::None)
            continue;
        pos = game.actors.pos(i);
        if (pos.z != game.camera_depth)
            continue;

        x = pos.x - game.camera_x;
        y = pos.y - game.camera_y;

        if (x < 0 || x >= size.x || y < 0 || y >= size.y)
            continue;

        const ActorData& actor_data = ActorData::from_id(actor_id);
        wish_move(game.screen.screen, x, y);
        wish_color(&attr, actor_data.color);
        wish_bcolor(&attr, 0);
        wish_putchar(game.screen.screen, actor_data.sym, attr);
    }
}

static void render_items(Game& game)
{
    wish_attr attr;
    wish_size size;
    int count;
    int x;
    int y;

    wish_attr_init(&attr);
    wish_get_view_size(game.screen.screen, &size);
    count = game.items.count();
    for (int i = 0; i < count; ++i)
    {
        ItemID item_id;
        Vec3 pos;

        item_id = game.items.item_id(i);
        if (item_id == ItemID::None)
            continue;
        pos = game.items.pos(i);
        if (pos.z != game.camera_depth)
            continue;

        x = pos.x - game.camera_x;
        y = pos.y - game.camera_y;

        if (x < 0 || x >= size.x || y < 0 || y >= size.y)
            continue;

        const ItemData& item_data = ItemData::from_id(item_id);
        wish_move(game.screen.screen, x, y);
        wish_color(&attr, item_data.color);
        wish_bcolor(&attr, 0);
        wish_putchar(game.screen.screen, item_data.sym, attr);
    }
}

void game_render(Game& game)
{
    game.fps_counter_render.update();

    wish_clear(game.screen.term);
    render_top_bar(game);
    render_map(game);
    render_actors(game);
    render_items(game);
    wish_draw(game.screen.term);
}
