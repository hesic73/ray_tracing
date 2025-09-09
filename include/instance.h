#pragma once

#include <memory>
#include <cmath>

#include "hittable.h"
#include "math/mat4.h"
#include "motion.h"
#include "aabb.h"

struct Transform
{
    Mat4 matrix;
    Mat4 inverse;
    Mat4 inverse_transpose;
    Motion motion;
    FloatType time0;

    Transform(const Mat4 &m = Mat4::identity(), const Motion &motion = Motion(), FloatType time0 = zero_f)
        : matrix(m), inverse(m.inverse()), inverse_transpose(inverse.transpose()), motion(motion), time0(time0) {}
};

inline AABB transform_aabb(const AABB &box, const Mat4 &m)
{
    Point3 p[8] = {
        {box.x.min, box.y.min, box.z.min},
        {box.x.min, box.y.min, box.z.max},
        {box.x.min, box.y.max, box.z.min},
        {box.x.min, box.y.max, box.z.max},
        {box.x.max, box.y.min, box.z.min},
        {box.x.max, box.y.min, box.z.max},
        {box.x.max, box.y.max, box.z.min},
        {box.x.max, box.y.max, box.z.max}
    };
    Point3 q = m.transform_point(p[0]);
    Point3 min = q;
    Point3 max = q;
    for (int i = 1; i < 8; ++i)
    {
        q = m.transform_point(p[i]);
        min.x = std::fmin(min.x, q.x); min.y = std::fmin(min.y, q.y); min.z = std::fmin(min.z, q.z);
        max.x = std::fmax(max.x, q.x); max.y = std::fmax(max.y, q.y); max.z = std::fmax(max.z, q.z);
    }
    return AABB(min, max);
}

struct Instance : public Hittable
{
    std::shared_ptr<Hittable> object;
    std::shared_ptr<Transform> transform;

    Instance(const std::shared_ptr<Hittable> &object, const std::shared_ptr<Transform> &transform)
        : object(object), transform(transform) {}

    bool hit(const Ray &r, Interval t_range, HitRecord &rec) const override
    {
        Vec3 offset = (r.time - transform->time0) * transform->motion.linear;
        Point3 origin = transform->inverse.transform_point(r.origin - offset);
        Vec3 direction = transform->inverse.transform_vector(r.direction);
        Ray local_ray(origin, direction, r.time);
        if (!object->hit(local_ray, t_range, rec))
            return false;
        rec.point = transform->matrix.transform_point(rec.point) + offset;
        Vec3 n = transform->inverse_transpose.transform_vector(rec.normal);
        rec.set_face_normal(r, Vec3::normalize(n));
        return true;
    }

    AABB bounding_box() const override
    {
        return transform_aabb(object->bounding_box(), transform->matrix);
    }

    AABB bounding_box(FloatType t1) const override
    {
        // NOTE: Only linear translation is accounted for here; any rotational
        // motion is ignored and may lead to an underestimated bounding box.
        AABB box0 = transform_aabb(object->bounding_box(), transform->matrix);
        Mat4 m1 = transform->matrix;
        Vec3 trans = (t1 - transform->time0) * transform->motion.linear;
        m1.set_translation(m1.get_translation() + trans);
        AABB box1 = transform_aabb(object->bounding_box(), m1);
        return AABB::surrounding_box(box0, box1);
    }
};

