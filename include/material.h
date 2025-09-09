#pragma once

#include "hittable.h"
#include "ray.h"
#include "color.h"
#include "rand_utils.h"
#include "texture.h"

struct Material
{
    virtual ~Material() = default;

    virtual bool scatter(
        const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const = 0;

    virtual Color emitted(FloatType u, FloatType v, const Point3 &p) const
    {
        return Color::black();
    }
};

struct Lambertian : public Material
{
    Lambertian(const Texture *albedo) : albedo(albedo) {}

    bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override
    {
        auto scatter_direction = rec.normal + random_unit_sphere();
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;
        scattered = Ray(rec.point, scatter_direction, r_in.time);
        attenuation = albedo->value(rec.u, rec.v, rec.point);
        return true;
    }
    const Texture *albedo;
};

struct Metal : public Material
{
    Metal(const Color &albedo, FloatType fuzz)
        : albedo(albedo), fuzz(fuzz < 0 ? 0 : (fuzz < 1 ? fuzz : 1)) {}

    bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override
    {
        Vec3 reflected = Vec3::reflect(Vec3::normalize(r_in.direction), rec.normal);
        scattered = Ray(rec.point, reflected + fuzz * random_unit_sphere(), r_in.time);
        attenuation = albedo;
        return (Vec3::dot(scattered.direction, rec.normal) > 0);
    }

    Color albedo;
    FloatType fuzz;
};

struct Dielectric : public Material
{
    Dielectric(FloatType refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override
    {
        attenuation = Color(1.0, 1.0, 1.0);
        FloatType ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

        Vec3 unit_direction = Vec3::normalize(r_in.direction);
        FloatType cos_theta = std::fmin(Vec3::dot(-unit_direction, rec.normal), one_f);
        FloatType sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = ri * sin_theta > 1.0;
        if (cannot_refract || reflectance(cos_theta, ri) > random_float())
        {
            Vec3 reflected = Vec3::reflect(unit_direction, rec.normal);
            scattered = Ray(rec.point, reflected, r_in.time);
            return true;
        }
        else
        {
            Vec3 refracted = Vec3::refract(unit_direction, rec.normal, ri);
            scattered = Ray(rec.point, refracted, r_in.time);
            return true;
        }
    }

    FloatType refraction_index;

    static FloatType reflectance(FloatType cosine, FloatType refraction_index)
    {
        // Use Schlick's approximation for reflectance.
        FloatType r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};

struct DiffuseLight : public Material
{
    DiffuseLight(const Texture *emit) : emit(emit) {}

    bool scatter(const Ray &, const HitRecord &, Color &, Ray &) const override
    {
        return false;
    }

    Color emitted(FloatType u, FloatType v, const Point3 &p) const override
    {
        return emit->value(u, v, p);
    }

    const Texture *emit;
};

