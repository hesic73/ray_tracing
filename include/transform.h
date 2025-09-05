#pragma once

#include "vec3.h"
#include "quaternion.h"

struct Transform
{
    Vec3 position;
    Quaternion rotation;

    Transform(const Vec3 &position = Vec3::zero(),
              const Quaternion &rotation = Quaternion::identity())
        : position(position), rotation(rotation) {}

    Transform(const Transform &t) = default;
    Transform &operator=(const Transform &t) = default;

    static Transform identity()
    {
        return Transform(Vec3::zero(), Quaternion::identity());
    }
};
