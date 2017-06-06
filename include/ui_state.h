#ifndef UI_STATE_H
#define UI_STATE_H

#include <cstdint>
#include <non_copyable.h>
#include <math/vector.h>

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
    Vector3i        cursor() const { return _cursor; }
    Vector3i        cursor_origin() const { return _cursor_origin; }

    void        set_state(UiStateID state) { _state = state; }
    void        set_cursor(Vector3i pos, bool origin = false);
    void        move_cursor(Vector3i delta);

private:
    UiStateID   _state;
    Vector3i        _cursor;
    Vector3i        _cursor_origin;
};

#endif
