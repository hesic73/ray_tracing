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

        Mat4 m = Mat4::identity();
        m.m[0][0] = right.x; m.m[0][1] = up.x; m.m[0][2] = -forward.x; m.m[0][3] = position.x;
        m.m[1][0] = right.y; m.m[1][1] = up.y; m.m[1][2] = -forward.y; m.m[1][3] = position.y;
        m.m[2][0] = right.z; m.m[2][1] = up.z; m.m[2][2] = -forward.z; m.m[2][3] = position.z;
        m.m[3][0] = m.m[3][1] = m.m[3][2] = zero_f; m.m[3][3] = one_f;
        return m;
    }
}
