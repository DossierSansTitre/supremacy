#ifndef UI_STATE_H
#define UI_STATE_H

#include <cstdint>
#include <non_copyable.h>
#include <vec3.h>

enum class UiStateID : uint8_t
{
    None,
    Selection
};

class UiState : private NonCopyable
{
public:
    UiState();
    ~UiState();

    UiStateID   state() const { return _state; }
    Vec3        cursor() const { return _cursor; }
    Vec3        cursor_origin() const { return _cursor_origin; }

    void        set_state(UiStateID state) { _state = state; }
    void        set_cursor(Vec3 pos, bool origin = false);
    void        move_cursor(Vec3 delta);

private:
    UiStateID   _state;
    Vec3        _cursor;
    Vec3        _cursor_origin;
};

#endif
