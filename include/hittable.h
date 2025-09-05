#pragma once

#include "ray.h"
#include "common.h"


struct HitRecord
{
    Point3 point; // point of intersection
    Vec3 normal;  // normal at the intersection point
    FloatType t;  // ray parameter at the intersection
    bool front_face; // whether the intersection is on the front face

    void set_face_normal(const Ray& r, const Vec3& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = Vec3::dot(r.direction, outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }

    static HitRecord default_record() {
        return HitRecord{Point3::zero(), Vec3::zero(), static_cast<FloatType>(0), true};
    }
};

struct Hittable
{
    virtual ~Hittable() = default;
    virtual bool hit(const Ray &r, FloatType t_min, FloatType t_max, HitRecord &hit_record) const = 0;
};