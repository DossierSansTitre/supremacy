#include <game.h>

void game_update(Game& game)
{
    game.fps_counter_update.update();
    game_ai(game);
}
