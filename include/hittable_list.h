#pragma once

#include <vector>
#include <memory>

#include "hittable.h"

struct HittableList : public Hittable
{
    std::vector<std::shared_ptr<Hittable>> objects;

    HittableList() = default;

    void clear() { objects.clear(); }

    void add(const std::shared_ptr<Hittable> &object) { objects.push_back(object); }

    bool hit(const Ray &r, FloatType t_min, FloatType t_max, HitRecord &hit_record) const override
    {
        auto temp_record = HitRecord::default_record();
        bool hit_anything = false;
        FloatType closest_so_far = t_max;

        for (const auto &object : objects)
        {
            if (object->hit(r, t_min, closest_so_far, temp_record))
            {
                hit_anything = true;
                closest_so_far = temp_record.t;
                hit_record = temp_record;
            }
        }

        return hit_anything;
    }
};