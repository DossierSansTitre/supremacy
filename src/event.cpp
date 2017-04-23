#include <game.h>

void game_event(Game& game)
{
    SDL_Event event;

    while (game.window.poll_event(event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    game.running = false;
                    break;
                case SDLK_q:
                    if (game.camera_depth < game.map.depth() - 1)
                        game.camera_depth++;
                    break;
                case SDLK_e:
                    if (game.camera_depth > 0)
                        game.camera_depth--;
                    break;
                case SDLK_w:
                    game.camera_y--;
                    break;
                case SDLK_s:
                    game.camera_y++;
                    break;
                case SDLK_a:
                    game.camera_x--;;
                    break;
                case SDLK_d:
                    game.camera_x++;
                    break;
            }
        }
    }
}
