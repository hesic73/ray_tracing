#pragma once

#include "vec3.h"
#include "quaternion.h"

struct Transform;

namespace MathUtils
{

    Vec3 rotate(const Quaternion &q, const Vec3 &v);

    Vec3 transform_point(const Transform &t, const Vec3 &point);

    Transform look_at(const Vec3 &position, const Vec3 &target, const Vec3 &world_up = Vec3(0, 1, 0));
}
