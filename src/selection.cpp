#include <selection.h>
#include <math/linear.h>

Selection::Selection()
: _state(State::Inactive)
{

}

Selection::~Selection()
{

}

Rect3i Selection::selected() const
{
    switch (_state)
    {
        case State::Inactive:
            return rect_from_points(_points[0], _points[1]);
        case State::First:
            return {_cursor, {1, 1, 1}};
        case State::Second:
            return rect_from_points(_points[0], _cursor);
    }
    /* should never happen */
    return {{0, 0, 0}, {0, 0, 0}};
}

void Selection::set_cursor(Vector3i cursor)
{
    _cursor = cursor;
}

void Selection::advance()
{
    switch (_state)
    {
        case State::Inactive:
            _state = State::First;
            break;
        case State::First:
            _points[0] = _cursor;
            _state = State::Second;
            break;
        case State::Second:
            _points[1] = _cursor;
            _state = State::Inactive;
            break;
    }
}

void Selection::reset()
{
    _state = State::Inactive;
}
