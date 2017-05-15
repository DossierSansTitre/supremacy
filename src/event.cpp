#include <game.h>

void game_event(Game& game)
{
    SDL_Event event;

    game.keyboard.tick();
    while (game.window.poll_event(event))
    {
        switch (event.type)
        {
            case SDL_KEYDOWN:
                game.keyboard.press_key(event.key.keysym.scancode);
                break;
            case SDL_KEYUP:
                game.keyboard.release_key(event.key.keysym.scancode);
                break;
            default:
                break;
        }
    }
}

