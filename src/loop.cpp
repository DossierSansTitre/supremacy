#include <chrono>
#include <thread>
#include <game.h>

void game_loop(Game& game)
{
    using Clock = std::chrono::high_resolution_clock;
    using Timepoint = Clock::time_point;

    static const int update_hz = 20;
    static const double update_delay = 1.0 / update_hz;
    double update_acc = 0.0;
    double delta;

    Timepoint t0;
    Timepoint t1;

    game.running = true;

    t0 = Clock::now();
    while (game.running)
    {
        game_draw(game);

        if (!game.window.focus())
            SDL_Delay(50);

        while (update_acc >= update_delay)
        {
            update_acc -= update_delay;
            game_event(game);
            game_update(game);
        }

        t1 = Clock::now();
        delta = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count();

        update_acc += delta;
        t0 = t1;
    }
}
