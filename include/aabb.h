#pragma once

#include <algorithm>
#include <cmath>

#include "interval.h"
#include "ray.h"

struct AABB
{
    Interval x;
    Interval y;
    Interval z;
    AABB(const Interval &x, const Interval &y, const Interval &z)
        : x(x), y(y), z(z) {}
    AABB(const Point3 &a, const Point3 &b)
        : x(Interval(std::fmin(a.x, b.x), std::fmax(a.x, b.x))),
          y(Interval(std::fmin(a.y, b.y), std::fmax(a.y, b.y))),
          z(Interval(std::fmin(a.z, b.z), std::fmax(a.z, b.z))) {}

    bool hit(const Ray &r, Interval t_range) const
    {
        for (int i = 0; i < 3; ++i) {
            FloatType invD = static_cast<FloatType>(1.0) / r.direction[i];
            FloatType t0 = (axis(i).min - r.origin[i]) * invD;
            FloatType t1 = (axis(i).max - r.origin[i]) * invD;
            if (invD < 0)
                std::swap(t0, t1);
            t_range.min = t0 > t_range.min ? t0 : t_range.min;
            t_range.max = t1 < t_range.max ? t1 : t_range.max;
            if (t_range.max <= t_range.min)
                return false;
        }
        return true;
    }

    static AABB surrounding_box(const AABB &box0, const AABB &box1)
    {
        return AABB(
            Interval(std::fmin(box0.x.min, box1.x.min), std::fmax(box0.x.max, box1.x.max)),
            Interval(std::fmin(box0.y.min, box1.y.min), std::fmax(box0.y.max, box1.y.max)),
            Interval(std::fmin(box0.z.min, box1.z.min), std::fmax(box0.z.max, box1.z.max)));
    }

    const Interval& axis(int n) const
    {
        if (n == 0) return x;
        if (n == 1) return y;
        return z;
    }

    int longest_axis() const
    {
        FloatType x_extent = x.max - x.min;
        FloatType y_extent = y.max - y.min;
        FloatType z_extent = z.max - z.min;
        
        if (x_extent >= y_extent && x_extent >= z_extent) {
            return 0;
        } else if (y_extent >= z_extent) {
            return 1;
        } else {
            return 2;
        }
    }

    static AABB empty()
    {
        return AABB(Interval::empty(), Interval::empty(), Interval::empty());
    }
};

