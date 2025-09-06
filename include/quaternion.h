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

    /**
     * Create quaternion from Euler angles in ZYX convention.
     *
     * @param roll  Rotation around X-axis (in radians)
     * @param pitch Rotation around Y-axis (in radians)
     * @param yaw   Rotation around Z-axis (in radians)
     */
    static Quaternion from_euler(FloatType roll, FloatType pitch, FloatType yaw)
    {
        FloatType cy = std::cos(yaw * static_cast<FloatType>(0.5));
        FloatType sy = std::sin(yaw * static_cast<FloatType>(0.5));
        FloatType cp = std::cos(pitch * static_cast<FloatType>(0.5));
        FloatType sp = std::sin(pitch * static_cast<FloatType>(0.5));
        FloatType cr = std::cos(roll * static_cast<FloatType>(0.5));
        FloatType sr = std::sin(roll * static_cast<FloatType>(0.5));

        return Quaternion(
            cr * cp * cy + sr * sp * sy,
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy);
    }

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
};
