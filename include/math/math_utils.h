#pragma once

#include "vec3.h"
#include "mat4.h"
#include "common.h"

namespace MathUtils
{
    FloatType degrees_to_radians(FloatType degrees);

    Vec3 transform_point(const Mat4 &m, const Vec3 &point);

    Mat4 look_at(const Vec3 &position, const Vec3 &target, const Vec3 &world_up = Vec3(0, 1, 0));

    Mat3 rotation_from_angular_velocity(const Vec3 &omega, FloatType dt);
}

