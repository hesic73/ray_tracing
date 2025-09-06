#pragma once

#include "hittable.h"
#include "ray.h"
#include "color.h"
#include "rand_utils.h"

struct Material
{
    virtual ~Material() = default;

    virtual bool scatter(
        const Ray &r_in, const HitRecord &rec, ColorFloat &attenuation, Ray &scattered) const
    {
        return false;
    }
};

struct Lambertian : public Material
{
    Lambertian(const ColorFloat &albedo) : albedo(albedo) {}

    bool scatter(const Ray &r_in, const HitRecord &rec, ColorFloat &attenuation, Ray &scattered) const override
    {
        auto scatter_direction = rec.normal + random_unit_sphere();
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;
        scattered = Ray(rec.point, scatter_direction);
        attenuation = albedo;
        return true;
    }
    ColorFloat albedo;
};

struct Metal : public Material
{
    Metal(const ColorFloat &albedo, FloatType fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const Ray &r_in, const HitRecord &rec, ColorFloat &attenuation, Ray &scattered) const override
    {
        Vec3 reflected = Vec3::reflect(Vec3::normalize(r_in.direction), rec.normal);
        scattered = Ray(rec.point, reflected + fuzz * random_unit_sphere());
        attenuation = albedo;
        return (Vec3::dot(scattered.direction, rec.normal) > 0);
    }

    ColorFloat albedo;
    FloatType fuzz;
};