#pragma once

#include "vec3.h"
#include <cmath>

struct Quaternion
{
    FloatType w, x, y, z;

    constexpr Quaternion(FloatType w, FloatType x, FloatType y, FloatType z) : w(w), x(x), y(y), z(z) {}
    Quaternion(const Quaternion &q) = default;
    Quaternion &operator=(const Quaternion &q) = default;

    static Quaternion identity() { return Quaternion(1, 0, 0, 0); }

    static Quaternion normalize(const Quaternion &q)
    {
        FloatType n = q.norm();
        return Quaternion(q.w / n, q.x / n, q.y / n, q.z / n);
    }

    Quaternion operator*(const Quaternion &q) const
    {
        return Quaternion(
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w);
    }

    Quaternion conjugate() const
    {
        return Quaternion(w, -x, -y, -z);
    }

    FloatType norm() const
    {
        return std::sqrt(w * w + x * x + y * y + z * z);
    }

    static Quaternion inverse(const Quaternion &q)
    {
        FloatType n = q.norm();
        Quaternion conj = q.conjugate();
        return Quaternion(conj.w / (n * n), conj.x / (n * n), conj.y / (n * n), conj.z / (n * n));
    }

    static Quaternion uninitialized()
    {
        return Quaternion();
    }

private:
    Quaternion() {}
};
