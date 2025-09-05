#pragma once

#include <cmath>
#include <string>
#include "common.h"

struct Vec3
{
    FloatType x;
    FloatType y;
    FloatType z;
    Vec3(FloatType x, FloatType y, FloatType z) : x(x), y(y), z(z) {}
    Vec3(const Vec3 &v) = default;
    Vec3 &operator=(const Vec3 &v) = default;

    static Vec3 zero() { return Vec3(0, 0, 0); }

    FloatType operator[](int i) const
    {
        return (&x)[i];
    }
    FloatType &operator[](int i)
    {
        return (&x)[i];
    }

    FloatType norm() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    FloatType squared_norm() const
    {
        return x * x + y * y + z * z;
    }

    Vec3 &operator+=(const Vec3 &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vec3 &operator-=(const Vec3 &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vec3 &operator*=(const FloatType t)
    {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    Vec3 &operator/=(const FloatType t)
    {
        x /= t;
        y /= t;
        z /= t;
        return *this;
    }

    Vec3 operator-() const
    {
        return Vec3(-x, -y, -z);
    }

    std::string to_string() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    }

    static Vec3 normalize(const Vec3 &v)
    {
        FloatType n = v.norm();
        return Vec3(v.x / n, v.y / n, v.z / n);
    }

    static FloatType dot(const Vec3 &u, const Vec3 &v)
    {
        return u.x * v.x + u.y * v.y + u.z * v.z;
    }

    static Vec3 cross(const Vec3 &u, const Vec3 &v)
    {
        return Vec3(u.y * v.z - u.z * v.y,
                    u.z * v.x - u.x * v.z,
                    u.x * v.y - u.y * v.x);
    }
};

Vec3 operator+(const Vec3 &u, const Vec3 &v);
Vec3 operator-(const Vec3 &u, const Vec3 &v);
Vec3 operator*(FloatType t, const Vec3 &v);
Vec3 operator*(const Vec3 &v, FloatType t);
Vec3 operator/(const Vec3 &v, FloatType t);

using Point3 = Vec3; // 3D point