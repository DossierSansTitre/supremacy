#include <game.h>

void game_event(Game& game)
{
    wish_unicode cp;

    cp = wish_getchar(game.screen.term);

    /* Detect Ctrl-C */
    if (cp == 3)
        game.running = false;
    else if (cp == '>' && game.camera_depth > 0)
        game.camera_depth--;
    else if (cp == '<' && game.camera_depth < game.map.depth() - 1)
        game.camera_depth++;
}
