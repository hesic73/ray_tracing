#pragma once

#include <memory>
#include <cmath>

#include "hittable.h"
#include "material.h"
#include "rand_utils.h"

struct ConstantMedium : public Hittable
{
    std::shared_ptr<Hittable> boundary;
    const Material *phase_function;
    FloatType neg_inv_density;

    ConstantMedium(const std::shared_ptr<Hittable> &boundary, FloatType density, const Material *phase_function)
        : boundary(boundary), phase_function(phase_function), neg_inv_density(-one_f / density) {}

    bool hit(const Ray &r, Interval t_range, HitRecord &rec) const override
    {
        HitRecord rec1 = HitRecord::uninitialized();
        HitRecord rec2 = HitRecord::uninitialized();

        if (!boundary->hit(r, Interval(-infinity_f, infinity_f), rec1))
            return false;
        if (!boundary->hit(r, Interval(rec1.t + 0.0001, infinity_f), rec2))
            return false;

        if (rec1.t < t_range.min)
            rec1.t = t_range.min;
        if (rec2.t > t_range.max)
            rec2.t = t_range.max;
        if (rec1.t >= rec2.t)
            return false;
        if (rec1.t < 0)
            rec1.t = 0;

        FloatType ray_length = r.direction.norm();
        FloatType distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
        FloatType hit_distance = neg_inv_density * std::log(random_float());

        if (hit_distance > distance_inside_boundary)
            return false;

        rec.t = rec1.t + hit_distance / ray_length;
        rec.point = r.at(rec.t);
        rec.normal = Vec3(1, 0, 0);
        rec.front_face = true;
        rec.material = phase_function;
        rec.u = 0;
        rec.v = 0;
        return true;
    }

    AABB bounding_box() const override { return boundary->bounding_box(); }
    AABB bounding_box(FloatType time1) const override { return boundary->bounding_box(time1); }
};

