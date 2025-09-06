#pragma once

#include <cstdint>

#include "vec3.h"
#include "ray.h"
#include "transform.h"
#include "math_utils.h"
#include "rand_utils.h"

struct Camera
{
    const std::int32_t image_width;
    const std::int32_t image_height;
    const FloatType aspect_ratio;

    const FloatType focal_dist;
    const FloatType defocus_radius;

    const FloatType viewport_height;
    const FloatType viewport_width;

    Transform pose;

    // Viewport and (u, v) convention:
    //   - The viewport is a rectangle in 3D space, at z = -focal_dist in camera local space.
    //   - (u, v) are normalized coordinates in [0, 1]:
    //       u = 0 is the left edge, u = 1 is the right edge
    //       v = 0 is the bottom edge, v = 1 is the top edge
    //   - get_ray(u, v) returns a ray from the camera origin through the (u,v) point on viewport.

    Camera(
        std::int32_t image_width,
        std::int32_t image_height,
        FloatType fov,
        FloatType focal_dist = static_cast<FloatType>(1.0),
        FloatType defocus_angle = static_cast<FloatType>(0.0),
        const Transform &pose = Transform::identity())
        : image_width(image_width),
          image_height(image_height),
          aspect_ratio(static_cast<FloatType>(image_width) / image_height),
          focal_dist(focal_dist),
          defocus_radius(focal_dist * std::tan(MathUtils::degrees_to_radians(defocus_angle) / static_cast<FloatType>(2.0))),
          viewport_height(static_cast<FloatType>(2.0) * std::tan(MathUtils::degrees_to_radians(fov) / static_cast<FloatType>(2.0)) * focal_dist),
          viewport_width(aspect_ratio * viewport_height),
          pose(pose)
    {
    }

    void look_at(const Vec3 &target, const Vec3 &world_up = Vec3(0, 1, 0))
    {
        pose = MathUtils::look_at(pose.position, target, world_up);
    }

    void set_position(const Vec3 &position)
    {
        pose.position = position;
    }

    void set_rotation(const Quaternion &rotation)
    {
        pose.rotation = rotation;
    }

    Ray get_ray(FloatType u, FloatType v) const
    {
        Vec3 local_point = Vec3(
            (u - static_cast<FloatType>(0.5)) * viewport_width,
            (v - static_cast<FloatType>(0.5)) * viewport_height,
            -focal_dist);

        Vec3 world_point = MathUtils::transform_point(pose, local_point);

        Vec3 ray_origin = pose.position;
        if (defocus_radius > static_cast<FloatType>(1e-8))
        {
            Vec3 p = random_in_unit_disk();
            Vec3 defocus_offset = defocus_radius * p;
            ray_origin = pose.position + defocus_offset;
        }

        Vec3 direction = Vec3::normalize(world_point - ray_origin);
        return Ray(ray_origin, direction);
    }
};