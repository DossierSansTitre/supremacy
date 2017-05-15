#ifndef RECT3_H
#define RECT3_H

#include <vec3.h>

struct Rect3
{
    Vec3    origin;
    Vec3    size;
};

inline Rect3 rect_from_points(Vec3 a, Vec3 b)
{
    Rect3 r;

    if (a.x < b.x)
    {
        r.origin.x = a.x;
        r.size.x = b.x - a.x + 1;
    }
    else
    {
        r.origin.x = b.x;
        r.size.x = a.x - b.x + 1;
    }

    if (a.y < b.y)
    {
        r.origin.y = a.y;
        r.size.y = b.y - a.y + 1;
    }
    else
    {
        r.origin.y = b.y;
        r.size.y = a.y - b.y + 1;
    }

    if (a.z < b.z)
    {
        r.origin.z = a.z;
        r.size.z = b.z - a.z + 1;
    }
    else
    {
        r.origin.z = b.z;
        r.size.z = a.z - b.z + 1;
    }

    return r;
}

#endif
