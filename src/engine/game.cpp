#include <engine/game.h>

Game::Game(Window& window)
: _window(window)
, _engine(window, _keyboard)
{

}

Game::~Game()
{

}
