#ifndef ENGINE_INPUT_H
#define ENGINE_INPUT_H

#include <std/array.h>
#include <engine/input_event.h>
#include <non_copyable.h>

class Input : private NonCopyable
{
public:
    Input();
    ~Input();

    void    tick();
    void    dispatch(InputEvent e);
    bool    poll(InputEvent& e);

private:
    uint32_t            _tick;
    Array<InputEvent>   _events;
    size_t              _event_cursor;
};

#endif
