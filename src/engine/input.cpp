#include <engine/input.h>

Input::Input()
: _tick(0)
, _event_cursor(0)
{

}

Input::~Input()
{

}

void Input::tick()
{
    _events.clear();
    _event_cursor = 0;
    _tick++;
}

void Input::dispatch(InputEvent e)
{
    _events.push_back(e);
}

bool Input::poll(InputEvent& e)
{
    if (_event_cursor >= _events.size())
        return false;
    e = _events[_event_cursor++];
    return true;
}
