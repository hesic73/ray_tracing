#include "math_utils.h"
#include "transform.h"
#include <cmath>
#include <numbers>

namespace MathUtils
{

    FloatType degrees_to_radians(FloatType degrees)
    {
        return degrees * static_cast<FloatType>(std::numbers::pi) / static_cast<FloatType>(180.0);
    }

    Vec3 rotate(const Quaternion &q, const Vec3 &v)
    {
        Vec3 u(q.x, q.y, q.z);
        FloatType s = q.w;
        return static_cast<FloatType>(2.0) * Vec3::dot(u, v) * u + (s * s - Vec3::dot(u, u)) * v + static_cast<FloatType>(2.0) * s * Vec3::cross(u, v);
    }

    Vec3 transform_point(const Transform &t, const Vec3 &point)
    {
        Vec3 rotated = rotate(t.rotation, point);
        return rotated + t.position;
    }

    Transform look_at(const Vec3 &position, const Vec3 &target, const Vec3 &world_up)
    {
        Vec3 forward = Vec3::normalize(position - target);
        Vec3 right = Vec3::normalize(Vec3::cross(world_up, forward));
        Vec3 up = Vec3::cross(forward, right);

        FloatType yaw = std::atan2(forward.x, forward.z);
        FloatType pitch = std::asin(-forward.y);
        Quaternion rotation = Quaternion::from_euler(0, pitch, yaw);

        return Transform(position, rotation);
    }
}
