#pragma once

#include "ray.h"
#include "interval.h"
#include "common.h"

class Material;

struct HitRecord
{
    Point3 point;    // point of intersection
    Vec3 normal;     // normal at the intersection point
    FloatType t;     // ray parameter at the intersection
    bool front_face; // whether the intersection is on the front face

    const Material *material;

    void set_face_normal(const Ray &r, const Vec3 &outward_normal)
    {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = Vec3::dot(r.direction, outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }

    static HitRecord uninitialized()
    {
        return HitRecord();
    }

private:
    HitRecord() : point(Point3::uninitialized()), normal(Vec3::uninitialized()) {}
};

struct Hittable
{
    virtual ~Hittable() = default;
    virtual bool hit(const Ray &r, Interval t_range, HitRecord &hit_record) const = 0;
};