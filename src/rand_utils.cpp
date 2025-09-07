#include "rand_utils.h"

#include <random>

FloatType random_float()
{
    static std::uniform_real_distribution<FloatType> distribution(0.0, 1.0);
    static std::mt19937 generator(std::random_device{}());
    return distribution(generator);
}

FloatType random_float(FloatType min, FloatType max)
{
    return min + (max - min) * random_float();
}

Vec3 random_unit_sphere()
{
    while (true)
    {
        Vec3 p = Vec3(random_float(-1, 1), random_float(-1, 1), random_float(-1, 1));
        FloatType squared_norm = p.squared_norm();
        if (squared_norm < one_f && squared_norm > 1e-8)
            return Vec3::normalize(p);
    }
}

Vec3 random_on_hemisphere(const Vec3 &normal)
{
    Vec3 on_unit_sphere = random_unit_sphere();
    if (Vec3::dot(on_unit_sphere, normal) > zero_f)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

Vec3 random_in_unit_disk()
{
    while (true)
    {
        Vec3 p = Vec3(random_float(-1, 1), random_float(-1, 1), 0);
        FloatType squared_norm = p.squared_norm();
        if (squared_norm < one_f && squared_norm > 1e-8)
            return p;
    }
}