#ifndef ACTION_ID
#define ACTION_ID

#include <cstdint>

enum class ActionID : uint8_t
{
    None,
    Wander,
    Attack,
    Mine,
    Chop,
};

#endif
