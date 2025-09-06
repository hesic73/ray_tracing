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
        Vec3 forward = Vec3::normalize(target - position);
        Vec3 up_hint = world_up;
        if (std::fabs(Vec3::dot(forward, up_hint)) > static_cast<FloatType>(0.999))
        {
            up_hint = std::fabs(forward.y) < static_cast<FloatType>(0.999) ? Vec3(0, 1, 0) : Vec3(1, 0, 0);
        }

        Vec3 right = Vec3::normalize(Vec3::cross(forward, up_hint));
        Vec3 up = Vec3::cross(right, forward);

        // Build rotation matrix columns (camera local axes in world space):
        // R = [ right | up | -forward ]
        FloatType m00 = right.x, m01 = up.x, m02 = -forward.x;
        FloatType m10 = right.y, m11 = up.y, m12 = -forward.y;
        FloatType m20 = right.z, m21 = up.z, m22 = -forward.z;

        // Convert rotation matrix to quaternion (right-handed, column-major basis vectors).
        FloatType trace = m00 + m11 + m22;
        Quaternion q = Quaternion::uninitialized();
        if (trace > 0)
        {
            FloatType t = trace + static_cast<FloatType>(1.0);
            FloatType s = static_cast<FloatType>(0.5) / std::sqrt(t);
            q.w = static_cast<FloatType>(0.25) / s;
            q.x = (m21 - m12) * s;
            q.y = (m02 - m20) * s;
            q.z = (m10 - m01) * s;
        }
        else if (m00 > m11 && m00 > m22)
        {
            FloatType t = static_cast<FloatType>(1.0) + m00 - m11 - m22;
            FloatType s = static_cast<FloatType>(0.5) / std::sqrt(t);
            q.w = (m21 - m12) * s;
            q.x = static_cast<FloatType>(0.25) / s;
            q.y = (m01 + m10) * s;
            q.z = (m02 + m20) * s;
        }
        else if (m11 > m22)
        {
            FloatType t = static_cast<FloatType>(1.0) - m00 + m11 - m22;
            FloatType s = static_cast<FloatType>(0.5) / std::sqrt(t);
            q.w = (m02 - m20) * s;
            q.x = (m01 + m10) * s;
            q.y = static_cast<FloatType>(0.25) / s;
            q.z = (m12 + m21) * s;
        }
        else
        {
            FloatType t = static_cast<FloatType>(1.0) - m00 - m11 + m22;
            FloatType s = static_cast<FloatType>(0.5) / std::sqrt(t);
            q.w = (m10 - m01) * s;
            q.x = (m02 + m20) * s;
            q.y = (m12 + m21) * s;
            q.z = static_cast<FloatType>(0.25) / s;
        }

        q = Quaternion::normalize(q);
        return Transform(position, q);
    }
}
