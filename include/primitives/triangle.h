#pragma once

#include <cmath>

#include "math/vec3.h"
#include "hittable.h"
#include "aabb.h"
#include "material.h"
#include "common.h"

struct Triangle : public Hittable
{
    Point3 p0;
    Point3 p1;
    Point3 p2;
    Vec3 normal;
    FloatType D;
    const Material *material;
    AABB bbox;

    Triangle(const Point3 &a, const Point3 &b, const Point3 &c, const Material *material)
        : p0(a), p1(b), p2(c), normal(Vec3::zero()), D(zero_f), material(material),
          bbox(Point3(zero_f, zero_f, zero_f), Point3(zero_f, zero_f, zero_f))
    {
        Vec3 n = Vec3::cross(p1 - p0, p2 - p0);
        normal = Vec3::normalize(n);
        D = Vec3::dot(normal, p0);
        FloatType minx = std::fmin(std::fmin(p0.x, p1.x), p2.x);
        FloatType miny = std::fmin(std::fmin(p0.y, p1.y), p2.y);
        FloatType minz = std::fmin(std::fmin(p0.z, p1.z), p2.z);
        FloatType maxx = std::fmax(std::fmax(p0.x, p1.x), p2.x);
        FloatType maxy = std::fmax(std::fmax(p0.y, p1.y), p2.y);
        FloatType maxz = std::fmax(std::fmax(p0.z, p1.z), p2.z);
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
        Vec3 c0 = Vec3::cross(p1 - p0, p - p0);
        Vec3 c1 = Vec3::cross(p2 - p1, p - p1);
        Vec3 c2 = Vec3::cross(p0 - p2, p - p2);
        if (Vec3::dot(normal, c0) < 0 || Vec3::dot(normal, c1) < 0 || Vec3::dot(normal, c2) < 0)
            return false;
        Vec3 v0 = p1 - p0;
        Vec3 v1 = p2 - p0;
        Vec3 v2 = p - p0;
        FloatType d00 = Vec3::dot(v0, v0);
        FloatType d01 = Vec3::dot(v0, v1);
        FloatType d11 = Vec3::dot(v1, v1);
        FloatType d20 = Vec3::dot(v2, v0);
        FloatType d21 = Vec3::dot(v2, v1);
        FloatType denom_bc = d00 * d11 - d01 * d01;
        FloatType v = (d11 * d20 - d01 * d21) / denom_bc;
        FloatType w = (d00 * d21 - d01 * d20) / denom_bc;
        FloatType u = one_f - v - w;
        if (u < 0 || v < 0 || w < 0)
            return false;
        hit_record.t = t;
        hit_record.point = p;
        hit_record.u = v;
        hit_record.v = w;
        hit_record.material = material;
        hit_record.set_face_normal(r, normal);
        return true;
    }

    AABB bounding_box() const override { return bbox; }
    AABB bounding_box(FloatType) const override { return bbox; }
};

