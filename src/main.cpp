#include <screen.h>

static void init_screen(Screen& screen)
{
    screen.term = wish_init();
    screen.screen = wish_view_create(screen.term);
    wish_view_fullscreen(screen.screen);
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

    Screen screen;

    init_screen(screen);
    destroy_screen(screen);

    return 0;
}
