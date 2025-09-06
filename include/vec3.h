#pragma once

#include <cmath>
#include <string>
#include "common.h"

struct Vec3;

Vec3 operator+(const Vec3 &u, const Vec3 &v);
Vec3 operator-(const Vec3 &u, const Vec3 &v);
Vec3 operator*(FloatType t, const Vec3 &v);
Vec3 operator*(const Vec3 &v, FloatType t);
Vec3 operator/(const Vec3 &v, FloatType t);

struct Vec3
{
    FloatType x;
    FloatType y;
    FloatType z;
    constexpr Vec3(FloatType x, FloatType y, FloatType z) : x(x), y(y), z(z) {}
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

    bool near_zero(FloatType epsilon = 1e-8) const
    {
        return (std::fabs(x) < epsilon) && (std::fabs(y) < epsilon) && (std::fabs(z) < epsilon);
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

    static Vec3 pow(const Vec3 &v, FloatType exponent)
    {
        return Vec3(std::pow(v.x, exponent), std::pow(v.y, exponent), std::pow(v.z, exponent));
    }

    static Vec3 reflect(const Vec3 &v, const Vec3 &n)
    {
        return v - 2 * Vec3::dot(v, n) * n;
    }

    static Vec3 refract(const Vec3 &uv, const Vec3 &n, FloatType etai_over_etat)
    {
        FloatType cos_theta = std::fmin(Vec3::dot(-uv, n), one_f);
        Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
        Vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.squared_norm())) * n;
        return r_out_perp + r_out_parallel;
    }

    static Vec3 uninitialized()
    {
        return Vec3();
    }

private:
    Vec3() {}
};

using Point3 = Vec3; // 3D point