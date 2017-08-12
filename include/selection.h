#ifndef SELECTION_H
#define SELECTION_H

#include <math/rect.h>

class Selection
{
public:
    enum class State
    {
        Inactive,
        First,
        Second
    };

    Selection();
    ~Selection();

    State       state() const { return _state; }
    Vector3i    cursor() const { return _cursor; }
    Rect3i      selected() const;

    void    set_cursor(Vector3i cursor);
    void    advance();
    void    reset();

private:
    State       _state;
    Vector3i    _points[2];
    Vector3i    _cursor;
};

#endif
