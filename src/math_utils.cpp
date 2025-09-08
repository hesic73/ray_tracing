#include "math/math_utils.h"
#include <cmath>
#include <numbers>

namespace MathUtils
{
    FloatType degrees_to_radians(FloatType degrees)
    {
        return degrees * static_cast<FloatType>(std::numbers::pi) / static_cast<FloatType>(180.0);
    }

    Vec3 transform_point(const Mat4 &m, const Vec3 &point)
    {
        return m.transform_point(point);
    }

    Mat4 look_at(const Vec3 &position, const Vec3 &target, const Vec3 &world_up)
    {
        Vec3 forward = Vec3::normalize(target - position);
        Vec3 up_hint = world_up;
        if (std::fabs(Vec3::dot(forward, up_hint)) > static_cast<FloatType>(0.999))
        {
            up_hint = std::fabs(forward.y) < static_cast<FloatType>(0.999) ? Vec3(0, 1, 0) : Vec3(1, 0, 0);
        }

        Vec3 right = Vec3::normalize(Vec3::cross(forward, up_hint));
        Vec3 up = Vec3::cross(right, forward);

        Mat3 rot = Mat3::from_columns(right, up, -forward);
        Mat4 m = Mat4::rotation(rot);
        m.set_translation(position);
        return m;
    }

    Mat3 rotation_from_angular_velocity(const Vec3 &omega, FloatType dt)
    {
        FloatType angle = omega.norm() * dt;
        if (angle == zero_f)
            return Mat3::identity();
        Vec3 axis = omega / omega.norm();
        FloatType c = std::cos(angle);
        FloatType s = std::sin(angle);
        FloatType t = one_f - c;
        FloatType x = axis.x, y = axis.y, z = axis.z;
        Mat3 r{};
        r.m[0][0] = t * x * x + c;
        r.m[0][1] = t * x * y - s * z;
        r.m[0][2] = t * x * z + s * y;
        r.m[1][0] = t * x * y + s * z;
        r.m[1][1] = t * y * y + c;
        r.m[1][2] = t * y * z - s * x;
        r.m[2][0] = t * x * z - s * y;
        r.m[2][1] = t * y * z + s * x;
        r.m[2][2] = t * z * z + c;
        return r;
    }
}
