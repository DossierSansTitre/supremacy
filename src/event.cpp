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
    else if (cp == 'w')
        game.camera_y--;
    else if (cp == 's')
        game.camera_y++;
    else if (cp == 'a')
        game.camera_x--;
    else if (cp == 'd')
        game.camera_x++;
    else if (cp == 'W')
        game.camera_y -= 20;
    else if (cp == 'S')
        game.camera_y += 20;
    else if (cp == 'A')
        game.camera_x -= 20;
    else if (cp == 'D')
        game.camera_x += 20;
}
