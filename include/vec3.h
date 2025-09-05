#pragma once

#include <cmath>
#include <string>

struct Vec3
{
    double x;
    double y;
    double z;
    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}
    Vec3(const Vec3 &v) = default;
    Vec3 &operator=(const Vec3 &v) = default;

    double operator[](int i) const
    {
        return (&x)[i];
    }
    double &operator[](int i)
    {
        return (&x)[i];
    }

    double norm() const
    {
        return std::sqrt(x * x + y * y + z * z);
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

    Vec3 &operator*=(const double t)
    {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    Vec3 &operator/=(const double t)
    {
        x /= t;
        y /= t;
        z /= t;
        return *this;
    }

    std::string to_string() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    }
};

Vec3 operator+(const Vec3 &u, const Vec3 &v);
Vec3 operator-(const Vec3 &u, const Vec3 &v);
Vec3 operator*(double t, const Vec3 &v);
Vec3 operator*(const Vec3 &v, double t);
Vec3 operator/(const Vec3 &v, double t);

double dot(const Vec3 &u, const Vec3 &v);
Vec3 cross(const Vec3 &u, const Vec3 &v);

Vec3 normalize(const Vec3 &v);