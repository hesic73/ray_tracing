#pragma once

#include <cmath>

#include "vec3.h"
#include "hittable.h"
#include "common.h"

struct Sphere : public Hittable
{
    Point3 center;
    FloatType radius;

    Sphere(const Point3 &center, FloatType radius) : center(center), radius(std::fmax(radius, FloatType(0))) {}

    bool hit(const Ray &r, Interval t_range, HitRecord &hit_record) const override
    {
        Vec3 oc = r.origin - center;
        FloatType a = r.direction.squared_norm();
        FloatType half_b = Vec3::dot(r.direction, oc);
        FloatType c = oc.squared_norm() - radius * radius;
        FloatType discriminant = half_b * half_b - a * c;

        if (discriminant < 0)
        {
            return false;
        }
        FloatType sqrt_d = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        FloatType root = (-half_b - sqrt_d) / a;
        if (!t_range.surrounds(root))
        {
            root = (-half_b + sqrt_d) / a;
            if (!t_range.surrounds(root))
            {
                return false;
            }
        }

        hit_record.t = root;
        hit_record.point = r.at(hit_record.t);
        Vec3 outward_normal = (hit_record.point - center) / radius;
        hit_record.set_face_normal(r, outward_normal);

        return true;
    }
};