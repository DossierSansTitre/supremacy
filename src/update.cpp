#include <game.h>

void game_update(Game& game)
{
    if (game.keyboard.pressed(SDL_SCANCODE_ESCAPE))
        game.running = false;
    game.fps_counter_update.update();
    game_ai(game);
}
