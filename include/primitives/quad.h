#pragma once

#include <cmath>

#include "math/vec3.h"
#include "hittable.h"
#include "aabb.h"
#include "material.h"
#include "common.h"

struct Quad : public Hittable
{
    Point3 q;
    Vec3 u;
    Vec3 v;
    Vec3 normal;
    Vec3 w;
    FloatType D;
    const Material *material;
    AABB bbox;

    Quad(const Point3 &q, const Vec3 &u, const Vec3 &v, const Material *material)
        : q(q), u(u), v(v), normal(Vec3::zero()), w(Vec3::zero()), D(zero_f), material(material),
          bbox(Point3(zero_f, zero_f, zero_f), Point3(zero_f, zero_f, zero_f))
    {
        Vec3 n = Vec3::cross(u, v);
        normal = Vec3::normalize(n);
        D = Vec3::dot(normal, q);
        w = n / n.squared_norm();
        Point3 p1 = q + u;
        Point3 p2 = q + v;
        Point3 p3 = q + u + v;
        FloatType minx = std::fmin(std::fmin(q.x, p1.x), std::fmin(p2.x, p3.x));
        FloatType miny = std::fmin(std::fmin(q.y, p1.y), std::fmin(p2.y, p3.y));
        FloatType minz = std::fmin(std::fmin(q.z, p1.z), std::fmin(p2.z, p3.z));
        FloatType maxx = std::fmax(std::fmax(q.x, p1.x), std::fmax(p2.x, p3.x));
        FloatType maxy = std::fmax(std::fmax(q.y, p1.y), std::fmax(p2.y, p3.y));
        FloatType maxz = std::fmax(std::fmax(q.z, p1.z), std::fmax(p2.z, p3.z));
        constexpr FloatType eps = static_cast<FloatType>(0.0001); // ensure non-zero extent
        if (maxx - minx < eps) {
            FloatType pad = eps / static_cast<FloatType>(2.0);
            minx -= pad;
            maxx += pad;
        }
        if (maxy - miny < eps) {
            FloatType pad = eps / static_cast<FloatType>(2.0);
            miny -= pad;
            maxy += pad;
        }
        if (maxz - minz < eps) {
            FloatType pad = eps / static_cast<FloatType>(2.0);
            minz -= pad;
            maxz += pad;
        }
        bbox = AABB(Point3(minx, miny, minz), Point3(maxx, maxy, maxz));
    }

    bool hit(const Ray &r, Interval t_range, HitRecord &hit_record) const override
    {
        FloatType denom = Vec3::dot(normal, r.direction);
        if (std::fabs(denom) < 1e-8)
            return false;
        FloatType t = (D - Vec3::dot(normal, r.origin)) / denom;
        if (!t_range.surrounds(t))
            return false;
        Point3 p = r.at(t);
        Vec3 rvec = p - q;
        FloatType alpha = Vec3::dot(w, Vec3::cross(rvec, v));
        FloatType beta = Vec3::dot(w, Vec3::cross(u, rvec));
        if (alpha < 0 || alpha > 1 || beta < 0 || beta > 1)
            return false;
        hit_record.t = t;
        hit_record.point = p;
        hit_record.u = alpha;
        hit_record.v = beta;
        hit_record.material = material;
        hit_record.set_face_normal(r, normal);
        return true;
    }

    AABB bounding_box() const override { return bbox; }
    AABB bounding_box(FloatType) const override { return bbox; }
};

