#include <wish.h>
#include <game.h>
#include <tile.h>
#include <material.h>

void game_render(Game& game)
{
    int x;
    int y;

    int fps_render;
    int fps_update;

    bool under;
    wish_attr attr;
    wish_size size;

    game.fps_counter_render.update();

    wish_attr_init(&attr);
    wish_get_view_size(game.screen.screen, &size);
    wish_clear(game.screen.term);

    fps_render = game.fps_counter_render.get();
    fps_update = game.fps_counter_update.get();

    wish_color(&attr, 15);
    wish_move(game.screen.top_bar, 0, 0);
    wish_printf(game.screen.top_bar, "FPS: %d(%d)", attr, fps_render, fps_update);

    wish_color(&attr, 9);
    wish_move(game.screen.top_bar, size.x / 2 - 4, 0);
    wish_puts(game.screen.top_bar, "SUPREMACY", attr);
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

    wish_draw(game.screen.term);
}
