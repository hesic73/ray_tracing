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

    static Mat3 from_columns(const Vec3 &c0, const Vec3 &c1, const Vec3 &c2)
    {
        Mat3 r{};
        r.m[0][0] = c0.x; r.m[1][0] = c0.y; r.m[2][0] = c0.z;
        r.m[0][1] = c1.x; r.m[1][1] = c1.y; r.m[2][1] = c1.z;
        r.m[0][2] = c2.x; r.m[1][2] = c2.y; r.m[2][2] = c2.z;
        return r;
    }

    Vec3 operator*(const Vec3 &v) const
    {
        return Vec3(
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z);
    }

    Mat3 operator*(const Mat3 &o) const
    {
        Mat3 r{};
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
            {
                r.m[i][j] = zero_f;
                for (int k = 0; k < 3; ++k)
                    r.m[i][j] += m[i][k] * o.m[k][j];
            }
        return r;
    }
};

