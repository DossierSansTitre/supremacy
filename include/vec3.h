#ifndef VEC3_H
#define VEC3_H

struct Vec3 {
    int     x;
    int     y;
    int     z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(int x, int y, int z) : x(x), y(y), z(z) {}

    Vec3& operator+=(Vec3 other)
    {
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }

    Vec3& operator-=(Vec3 other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;

        return *this;
    }
};

inline Vec3 operator+(Vec3 lhs, Vec3 rhs)
{
    return (lhs += rhs);
}

inline Vec3 operator-(Vec3 lhs, Vec3 rhs)
{
    return (lhs -= rhs);
}

inline bool operator<(Vec3 lhs, Vec3 rhs)
{
    if (lhs.x != rhs.x)
        return (lhs.x < rhs.x);
    if (lhs.y != rhs.y)
        return (lhs.y < rhs.y);
    return (lhs.z < rhs.z);
}

#endif
