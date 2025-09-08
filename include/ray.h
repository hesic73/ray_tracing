#pragma once

#include "math/vec3.h"

struct Ray
{
    Point3 origin;
    Vec3 direction;
    FloatType time;

    Ray(const Point3 &origin, const Vec3 &direction, FloatType time = zero_f)
        : origin(origin), direction(direction), time(time) {}

    Point3 at(FloatType t) const
    {
        return origin + t * direction;
    }

    static Ray uninitialized()
    {
        return Ray();
    }

private:
    Ray() : origin(Point3::uninitialized()), direction(Vec3::uninitialized()), time(zero_f) {}
};
