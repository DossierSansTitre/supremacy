#include <game.h>
#include <wish.h>

void game_render(Game& game)
{
    wish_attr attr;

    wish_attr_init(&attr);
    wish_color(&attr, 7);
    wish_clear(game.screen.term);
    wish_move(game.screen.screen, 0, 0);
    wish_printf(game.screen.screen, "%d", attr, game.debug);
    wish_draw(game.screen.term);
}
