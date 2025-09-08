#pragma once

#include "math/vec3.h"

struct Motion
{
    Vec3 linear;
    Vec3 angular;
    Motion(const Vec3 &linear = Vec3(0, 0, 0), const Vec3 &angular = Vec3(0, 0, 0))
        : linear(linear), angular(angular) {}
};

