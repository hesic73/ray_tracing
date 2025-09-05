#pragma once

#include <cstdint>

#include "vec3.h"
#include "ray.h"
#include "transform.h"
#include "math_utils.h"

struct Camera
{
    // Image dimensions
    const std::int32_t image_width;
    const std::int32_t image_height;
    const double aspect_ratio;

    const double viewport_height;
    const double viewport_width;
    const double focal_length;

    Transform pose;

    // Viewport and (u, v) convention:
    //   - The viewport is a rectangle in 3D space, at z = -focal_length in camera local space.
    //   - (u, v) are normalized coordinates in [0, 1]:
    //       u = 0 is the left edge, u = 1 is the right edge
    //       v = 0 is the bottom edge, v = 1 is the top edge
    //   - get_ray(u, v) returns a ray from the camera origin through the (u,v) point on viewport.

    Camera(
        std::int32_t image_width,
        std::int32_t image_height,
        double focal_length = 1.0,
        const Transform &pose = Transform::identity())
        : image_width(image_width),
          image_height(image_height),
          aspect_ratio(double(image_width) / image_height),
          viewport_height(2.0),
          viewport_width(aspect_ratio * viewport_height),
          focal_length(focal_length),
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

    Ray get_ray(double u, double v) const
    {
        Vec3 local_point = Vec3(
            (u - 0.5) * viewport_width,
            (v - 0.5) * viewport_height,
            -focal_length);

        Vec3 world_point = MathUtils::transform_point(pose, local_point);
        Vec3 direction = Vec3::normalize(world_point - pose.position);
        return Ray(pose.position, direction);
    }
};