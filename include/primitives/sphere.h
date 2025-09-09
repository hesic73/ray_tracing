#pragma once

#include <cmath>

#include "math/vec3.h"
#include "hittable.h"
#include "common.h"

struct Sphere : public Hittable
{
    Point3 center;
    FloatType radius;
    const Material *material;

    Sphere(const Point3 &center, FloatType radius, const Material *material)
        : center(center), radius(std::fmax(radius, FloatType(0))), material(material) {}

    bool hit(const Ray &r, Interval t_range, HitRecord &hit_record) const override
    {
        Vec3 oc = r.origin - center;
        FloatType a = r.direction.squared_norm();
        FloatType half_b = Vec3::dot(r.direction, oc);
        FloatType c = oc.squared_norm() - radius * radius;
        FloatType discriminant = half_b * half_b - a * c;
        if (discriminant < 0)
            return false;
        FloatType sqrt_d = std::sqrt(discriminant);
        FloatType root = (-half_b - sqrt_d) / a;
        if (!t_range.surrounds(root))
        {
            root = (-half_b + sqrt_d) / a;
            if (!t_range.surrounds(root))
                return false;
        }
        hit_record.t = root;
        hit_record.point = r.at(hit_record.t);
        Vec3 outward_normal = (hit_record.point - center) / radius;
        hit_record.set_face_normal(r, outward_normal);
        get_sphere_uv(outward_normal, hit_record.u, hit_record.v);
        hit_record.material = material;
        return true;
    }

    AABB bounding_box() const override
    {
        Vec3 r_vec(radius, radius, radius);
        return AABB(center - r_vec, center + r_vec);
    }

    AABB bounding_box(FloatType) const override
    {
        return bounding_box();
    }

    static void get_sphere_uv(const Vec3 &p, FloatType &u, FloatType &v)
    {
        auto theta = std::acos(-p.y);
        auto phi = std::atan2(-p.z, p.x) + pi;
        u = phi / (2 * pi);
        v = theta / pi;
    }
};
