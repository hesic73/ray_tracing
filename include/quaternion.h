#pragma once

#include "vec3.h"
#include <cmath>

struct Quaternion
{
    double w, x, y, z;

    Quaternion(double w, double x, double y, double z) : w(w), x(x), y(y), z(z) {}
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
    static Quaternion from_euler(double roll, double pitch, double yaw)
    {
        double cy = std::cos(yaw * 0.5);
        double sy = std::sin(yaw * 0.5);
        double cp = std::cos(pitch * 0.5);
        double sp = std::sin(pitch * 0.5);
        double cr = std::cos(roll * 0.5);
        double sr = std::sin(roll * 0.5);

        return Quaternion(
            cr * cp * cy + sr * sp * sy,
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy);
    }

    static Quaternion normalize(const Quaternion &q)
    {
        double n = q.norm();
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

    double norm() const
    {
        return std::sqrt(w * w + x * x + y * y + z * z);
    }

    static Quaternion inverse(const Quaternion &q)
    {
        double n = q.norm();
        Quaternion conj = q.conjugate();
        return Quaternion(conj.w / (n * n), conj.x / (n * n), conj.y / (n * n), conj.z / (n * n));
    }
};
