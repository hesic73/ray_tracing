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

    // Apply the full affine transform to a point (translation included).
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

    // Transform a direction vector (translation ignored).
    Vec3 transform_vector(const Vec3 &v) const
    {
        return Vec3(
            m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z,
            m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z,
            m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z);
    }

    Mat4 transpose() const
    {
        Mat4 r = *this;
        std::swap(r.m[0][1], r.m[1][0]);
        std::swap(r.m[0][2], r.m[2][0]);
        std::swap(r.m[0][3], r.m[3][0]);
        std::swap(r.m[1][2], r.m[2][1]);
        std::swap(r.m[1][3], r.m[3][1]);
        std::swap(r.m[2][3], r.m[3][2]);
        return r;
    }

    Mat4 inverse() const
    {
        Mat4 inv{};
        const FloatType *a = &m[0][0];
        FloatType invm[16];

        invm[0] = a[5] * a[10] * a[15] - a[5] * a[11] * a[14] - a[9] * a[6] * a[15] + a[9] * a[7] * a[14] + a[13] * a[6] * a[11] - a[13] * a[7] * a[10];
        invm[4] = -a[4] * a[10] * a[15] + a[4] * a[11] * a[14] + a[8] * a[6] * a[15] - a[8] * a[7] * a[14] - a[12] * a[6] * a[11] + a[12] * a[7] * a[10];
        invm[8] = a[4] * a[9] * a[15] - a[4] * a[11] * a[13] - a[8] * a[5] * a[15] + a[8] * a[7] * a[13] + a[12] * a[5] * a[11] - a[12] * a[7] * a[9];
        invm[12] = -a[4] * a[9] * a[14] + a[4] * a[10] * a[13] + a[8] * a[5] * a[14] - a[8] * a[6] * a[13] - a[12] * a[5] * a[10] + a[12] * a[6] * a[9];
        invm[1] = -a[1] * a[10] * a[15] + a[1] * a[11] * a[14] + a[9] * a[2] * a[15] - a[9] * a[3] * a[14] - a[13] * a[2] * a[11] + a[13] * a[3] * a[10];
        invm[5] = a[0] * a[10] * a[15] - a[0] * a[11] * a[14] - a[8] * a[2] * a[15] + a[8] * a[3] * a[14] + a[12] * a[2] * a[11] - a[12] * a[3] * a[10];
        invm[9] = -a[0] * a[9] * a[15] + a[0] * a[11] * a[13] + a[8] * a[1] * a[15] - a[8] * a[3] * a[13] - a[12] * a[1] * a[11] + a[12] * a[3] * a[9];
        invm[13] = a[0] * a[9] * a[14] - a[0] * a[10] * a[13] - a[8] * a[1] * a[14] + a[8] * a[2] * a[13] + a[12] * a[1] * a[10] - a[12] * a[2] * a[9];
        invm[2] = a[1] * a[6] * a[15] - a[1] * a[7] * a[14] - a[5] * a[2] * a[15] + a[5] * a[3] * a[14] + a[13] * a[2] * a[7] - a[13] * a[3] * a[6];
        invm[6] = -a[0] * a[6] * a[15] + a[0] * a[7] * a[14] + a[4] * a[2] * a[15] - a[4] * a[3] * a[14] - a[12] * a[2] * a[7] + a[12] * a[3] * a[6];
        invm[10] = a[0] * a[5] * a[15] - a[0] * a[7] * a[13] - a[4] * a[1] * a[15] + a[4] * a[3] * a[13] + a[12] * a[1] * a[7] - a[12] * a[3] * a[5];
        invm[14] = -a[0] * a[5] * a[14] + a[0] * a[6] * a[13] + a[4] * a[1] * a[14] - a[4] * a[2] * a[13] - a[12] * a[1] * a[6] + a[12] * a[2] * a[5];
        invm[3] = -a[1] * a[6] * a[11] + a[1] * a[7] * a[10] + a[5] * a[2] * a[11] - a[5] * a[3] * a[10] - a[9] * a[2] * a[7] + a[9] * a[3] * a[6];
        invm[7] = a[0] * a[6] * a[11] - a[0] * a[7] * a[10] - a[4] * a[2] * a[11] + a[4] * a[3] * a[10] + a[8] * a[2] * a[7] - a[8] * a[3] * a[6];
        invm[11] = -a[0] * a[5] * a[11] + a[0] * a[7] * a[9] + a[4] * a[1] * a[11] - a[4] * a[3] * a[9] - a[8] * a[1] * a[7] + a[8] * a[3] * a[5];
        invm[15] = a[0] * a[5] * a[10] - a[0] * a[6] * a[9] - a[4] * a[1] * a[10] + a[4] * a[2] * a[9] + a[8] * a[1] * a[6] - a[8] * a[2] * a[5];

        FloatType det = a[0] * invm[0] + a[1] * invm[4] + a[2] * invm[8] + a[3] * invm[12];
        FloatType inv_det = one_f / det;
        for (int i = 0; i < 16; ++i)
            (&inv.m[0][0])[i] = invm[i] * inv_det;
        return inv;
    }

    void set_translation(const Vec3 &t)
    {
        m[0][3] = t.x;
        m[1][3] = t.y;
        m[2][3] = t.z;
    }

    Vec3 get_translation() const
    {
        return Vec3(m[0][3], m[1][3], m[2][3]);
    }

    void set_rotation(const Mat3 &r3)
    {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                m[i][j] = r3.m[i][j];
    }

    Mat3 get_rotation() const
    {
        Mat3 r{};
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                r.m[i][j] = m[i][j];
        return r;
    }
};

