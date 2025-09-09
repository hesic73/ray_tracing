#pragma once

#include <cmath>

#include "math/vec3.h"
#include "math/mat4.h"
#include "hittable.h"
#include "common.h"
#include "motion.h"

struct Sphere : public Hittable
{
    Mat4 transform;
    Motion motion;
    FloatType time0;
    FloatType radius;
    const Material *material;

    Sphere(const Mat4 &transform, FloatType radius, const Material *material, const Motion &motion = Motion(), FloatType time0 = zero_f)
        : transform(transform), motion(motion), time0(time0), radius(std::fmax(radius, FloatType(0))), material(material) {}

    Sphere(const Point3 &center, FloatType radius, const Material *material, const Motion &motion = Motion(), FloatType time0 = zero_f)
        : transform(Mat4::translation(center)), motion(motion), time0(time0), radius(std::fmax(radius, FloatType(0))), material(material) {}

    bool hit(const Ray &r, Interval t_range, HitRecord &hit_record) const override
    {
        Vec3 center_now = transform.get_translation() + (r.time - time0) * motion.linear;
        Vec3 oc = r.origin - center_now;
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
        Vec3 outward_normal = (hit_record.point - center_now) / radius;
        hit_record.set_face_normal(r, outward_normal);
        get_sphere_uv(outward_normal, hit_record.u, hit_record.v);
        hit_record.material = material;

        return true;
    }

    AABB bounding_box() const override
    {
        Vec3 center = transform.get_translation();
        Vec3 r_vec(radius, radius, radius);
        return AABB(center - r_vec, center + r_vec);
    }

    AABB bounding_box(FloatType t1) const override
    {
        Vec3 center0 = transform.get_translation();
        Vec3 center1 = transform.get_translation() + (t1 - time0) * motion.linear;
        Vec3 r_vec(radius, radius, radius);
        AABB box0(center0 - r_vec, center0 + r_vec);
        AABB box1(center1 - r_vec, center1 + r_vec);
        return AABB::surrounding_box(box0, box1);
    }

    static void get_sphere_uv(const Vec3 &p, FloatType &u, FloatType &v)
    {
        auto theta = std::acos(-p.y);
        auto phi = std::atan2(-p.z, p.x) + pi;
        u = phi / (2 * pi);
        v = theta / pi;
    }
};
