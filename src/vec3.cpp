#include "vec3.h"

// algebraic operations
Vec3 operator+(const Vec3 &u, const Vec3 &v)
{
    return Vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}
Vec3 operator-(const Vec3 &u, const Vec3 &v)
{
    return Vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}
Vec3 operator*(FloatType t, const Vec3 &v)
{
    return Vec3(t * v.x, t * v.y, t * v.z);
}
Vec3 operator*(const Vec3 &v, FloatType t)
{
    return Vec3(t * v.x, t * v.y, t * v.z);
}
Vec3 operator/(const Vec3 &v, FloatType t)
{
    return Vec3(v.x / t, v.y / t, v.z / t);
}
