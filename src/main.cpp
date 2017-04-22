#include <ctime>
#include <game.h>

static void init_screen(Screen& screen)
{
    wish_size size;

    screen.term = wish_init();
    wish_get_size(screen.term, &size);

    screen.screen = wish_view_create(screen.term);
    screen.top_bar = wish_view_create(screen.term);

    wish_view_resize(screen.screen, 0, 1, size.x, size.y - 1);
    wish_view_resize(screen.top_bar, 0, 0, size.x, 1);

    wish_read_block(screen.term, 0);
    wish_read_delay(screen.term, 0);
    wish_cursor_show(screen.term, 0);
}

static void destroy_screen(Screen& screen)
{
    wish_view_free(screen.top_bar);
    wish_view_free(screen.screen);
    wish_quit(screen.term);
}

#include <iostream>

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

    generate_map(game.map, game.seed);

    wish_puts(game.screen.screen, "Done\n", attr);
    wish_draw(game.screen.term);

    game.actors.add(ActorID::Dwarf, {0, 0, 0});
    game.actors.add(ActorID::Dwarf, {2, 2, 0});
    game.actors.add(ActorID::Goblin, {2, 0, 0});
    game.actors.add(ActorID::Goblin, {2, 1, 0});
 
    game.items.add(ItemID::Wood, {0, 1, 0}, 30);

    game_loop(game);
    destroy_screen(game.screen);

    return 0;
}
