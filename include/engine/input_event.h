#ifndef GAME_INPUT_EVENT_H
#define GAME_INPUT_EVENT_H

#include <types.h>

enum class InputEventType
{
    KeyDown,
    KeyUp,
    Text
};

struct InputEventKey
{
    uint32_t    scancode;
};

struct InputEventText
{
    uint32_t    unicode;
};

struct InputEvent
{
    InputEventType  type;
    union
    {
        InputEventKey   key;
        InputEventText  text;
    };
};

#endif
