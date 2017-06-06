#include <ui_state.h>

UiState::UiState()
: _state(UiStateID::None)
{

}

UiState::~UiState()
{

}

void UiState::set_cursor(Vector3i pos, bool origin)
{
    _cursor = pos;
    if (origin)
        _cursor_origin = pos;
}

void UiState::move_cursor(Vector3i delta)
{
    _cursor.x += delta.x;
    _cursor.y += delta.y;
    _cursor.z += delta.z;
}
