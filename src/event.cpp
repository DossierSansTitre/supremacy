#include <game.h>

void game_event(Game& game)
{
    wish_unicode cp;

    cp = wish_getchar(game.screen.term);

    /* Detect Ctrl-C */
    if (cp == 3)
    {
        game.running = false;
    }
}
