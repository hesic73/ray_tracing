#pragma once

#include "vec3.h"
#include "mat3.h"
#include "common.h"

struct Mat4
{
    FloatType m[4][4];

    static Mat4 identity()
    {
        Mat4 r{};
        r.m[0][0] = r.m[1][1] = r.m[2][2] = r.m[3][3] = one_f;
        r.m[0][1] = r.m[0][2] = r.m[0][3] = zero_f;
        r.m[1][0] = r.m[1][2] = r.m[1][3] = zero_f;
        r.m[2][0] = r.m[2][1] = r.m[2][3] = zero_f;
        r.m[3][0] = r.m[3][1] = r.m[3][2] = zero_f;
        return r;
    }

    static Mat4 translation(const Vec3 &t)
    {
        Mat4 r = identity();
        r.m[0][3] = t.x;
        r.m[1][3] = t.y;
        r.m[2][3] = t.z;
        return r;
    }

    static Mat4 scale(const Vec3 &s)
    {
        Mat4 r{};
        r.m[0][0] = s.x; r.m[1][1] = s.y; r.m[2][2] = s.z; r.m[3][3] = one_f;
        r.m[0][1] = r.m[0][2] = r.m[0][3] = zero_f;
        r.m[1][0] = r.m[1][2] = r.m[1][3] = zero_f;
        r.m[2][0] = r.m[2][1] = r.m[2][3] = zero_f;
        r.m[3][0] = r.m[3][1] = r.m[3][2] = zero_f;
        return r;
    }

    static Mat4 rotation(const Mat3 &r3)
    {
        Mat4 r = identity();
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                r.m[i][j] = r3.m[i][j];
        return r;
    }

    static Mat4 TRS(const Vec3 &t, const Mat3 &r, const Vec3 &s)
    {
        return translation(t) * rotation(r) * scale(s);
    }

    Mat4 operator*(const Mat4 &o) const
    {
        Mat4 r{};
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
            {
                r.m[i][j] = zero_f;
                for (int k = 0; k < 4; ++k)
                    r.m[i][j] += m[i][k] * o.m[k][j];
            }
        return r;
    }

    Vec3 transform_point(const Vec3 &v) const
    {
        FloatType x = m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z + m[0][3];
        FloatType y = m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z + m[1][3];
        FloatType z = m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z + m[2][3];
        FloatType w = m[3][0]*v.x + m[3][1]*v.y + m[3][2]*v.z + m[3][3];
        if (w != one_f && w != zero_f)
        {
            x /= w; y /= w; z /= w;
        }
        return Vec3(x, y, z);
    }

    Vec3 get_translation() const
    {
        return Vec3(m[0][3], m[1][3], m[2][3]);
    }
};

