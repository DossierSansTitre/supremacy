#include <game.h>

static void ai_wander(Game& game, int actor)
{
    Actors& actors = game.actors;
    Vec3 pos;
    int r;

    if (!actors.use_speed(actor, 250))
        return;

    pos = actors.pos(actor);
    r = rand() % 4;

    if (r == 0)
    {
        pos.x++;
    }
    else if (r == 1)
    {
        pos.x--;
    }
    else if (r == 2)
    {
        pos.y++;
    }
    else
    {
        pos.y--;
    }
    actors.set_pos(actor, pos);
}

void game_ai(Game& game)
{
    int count;
    Actors& actors = game.actors;

    count = actors.count();

    for (int i = 0; i < count; ++i)
    {
        actors.speed_tick(i);
        switch (actors.action(i))
        {
            case ActionID::Wander:
                ai_wander(game, i);
                break;
            default:
                break;
        }
    }
}
