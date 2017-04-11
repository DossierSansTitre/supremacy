#include <game.h>

static void init_screen(Screen& screen)
{
    screen.term = wish_init();
    screen.screen = wish_view_create(screen.term);
    wish_view_fullscreen(screen.screen);
    wish_read_block(screen.term, 0);
    wish_read_delay(screen.term, 0);
    wish_cursor_show(screen.term, 0);
}

static void destroy_screen(Screen& screen)
{
    wish_view_free(screen.screen);
    wish_quit(screen.term);
}

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    Game game;

    init_screen(game.screen);
    generate_map(game.map);
    game_loop(game);
    destroy_screen(game.screen);

    return 0;
}
