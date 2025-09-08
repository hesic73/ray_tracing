#pragma once

#include "vec3.h"
#include "common.h"

struct Mat3
{
    FloatType m[3][3];

    static Mat3 identity()
    {
        Mat3 r{};
        r.m[0][0] = r.m[1][1] = r.m[2][2] = one_f;
        r.m[0][1] = r.m[0][2] = r.m[1][0] = r.m[1][2] = r.m[2][0] = r.m[2][1] = zero_f;
        return r;
    }

    Vec3 operator*(const Vec3 &v) const
    {
        return Vec3(
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z);
    }
};

