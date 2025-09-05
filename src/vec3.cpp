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
Vec3 operator*(double t, const Vec3 &v)
{
    return Vec3(t * v.x, t * v.y, t * v.z);
}
Vec3 operator*(const Vec3 &v, double t)
{
    return Vec3(t * v.x, t * v.y, t * v.z);
}
Vec3 operator/(const Vec3 &v, double t)
{
    return Vec3(v.x / t, v.y / t, v.z / t);
}

// dot product and cross product
double dot(const Vec3 &u, const Vec3 &v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

Vec3 cross(const Vec3 &u, const Vec3 &v)
{
    return Vec3(u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x);
}

// normalization
Vec3 normalize(const Vec3 &v)
{
    return v / v.norm();
}