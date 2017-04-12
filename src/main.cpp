#include <ctime>
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

    wish_attr attr;
    Game game;

    // KLUDGE
    game.camera_depth = 0;
    game.camera_x = 0;
    game.camera_y = 0;
    game.seed = time(nullptr);
    init_screen(game.screen);

    wish_attr_init(&attr);
    wish_color(&attr, 15);
    wish_puts(game.screen.screen, "Generating map...\n", attr);
    wish_draw(game.screen.term);

    wish_puts(game.screen.screen, "Done\n", attr);
    wish_draw(game.screen.term);

    generate_map(game.map, game.seed);
    game_loop(game);
    destroy_screen(game.screen);

    return 0;
}
