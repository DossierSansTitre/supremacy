#include <chrono>
#include <thread>
#include <game.h>

void game_loop(Game& game)
{
    using Clock = std::chrono::high_resolution_clock;
    using Timepoint = Clock::time_point;

    static const int render_hz = 60;
    static const int update_hz = 20;

    static const double render_delay = 1.0 / render_hz;
    static const double update_delay = 1.0 / update_hz;

    double render_acc = 0.0;
    double update_acc = 0.0;

    double render_wait;
    double update_wait;
    double wait;
    double delta;

    Timepoint t0;
    Timepoint t1;

    game.running = true;

    game_render(game);
    t0 = Clock::now();

    while (game.running)
    {
        while (render_acc >= render_delay)
        {
            render_acc -= render_delay;
            game_render(game);
        }

        while (update_acc >= update_delay)
        {
            update_acc -= update_delay;
            game_event(game);
            game_update(game);
        }

        render_wait = (render_delay - render_acc);
        update_wait = (update_delay - update_acc);

        if (render_wait > update_wait)
            wait = update_wait;
        else
            wait = render_wait;

        std::this_thread::sleep_for(std::chrono::duration<double>(wait));

        t1 = Clock::now();
        delta = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count();

        render_acc += delta;
        update_acc += delta;

        t0 = t1;
    }
}
