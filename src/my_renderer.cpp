#include "my_renderer.h"

#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "camera.h"
#include "common.h"
#include "rand_utils.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

Vec3 ray_color(const Ray &r, int depth, const Hittable &world)
{
    if (depth <= 0)
        return Vec3::zero();
    HitRecord hit_record = HitRecord::default_record();
    if (world.hit(r, Interval::interval_zero_infinity(), hit_record))
    {
        auto direction = random_on_hemisphere(hit_record.normal);
        return 0.5 * ray_color(Ray(hit_record.point, direction), depth - 1, world);
    }

    Vec3 unit_direction = Vec3::normalize(r.direction);
    auto a = static_cast<FloatType>(0.5) * (unit_direction.y + one_f);
    return (one_f - a) * Vec3(one_f, one_f, one_f) + a * Vec3(static_cast<FloatType>(0.5), static_cast<FloatType>(0.7), static_cast<FloatType>(1.0));
}

MyRenderer::MyRenderer(int samples_per_pixel, int max_depth)
    : samples_per_pixel(samples_per_pixel), max_depth(max_depth)
{
}

void MyRenderer::render(
    const Camera &camera,
    const Hittable &world,
    std::uint8_t *buffer) const
{
    for (int j = 0; j < camera.image_height; ++j)
    {
        for (int i = 0; i < camera.image_width; ++i)
        {
            Vec3 pixel_color_sum = Vec3::zero();

            for (int sample = 0; sample < samples_per_pixel; ++sample)
            {
                FloatType random_u = random_float() - static_cast<FloatType>(0.5); // [-0.5, 0.5)
                FloatType random_v = random_float() - static_cast<FloatType>(0.5); // [-0.5, 0.5)

                FloatType u = (static_cast<FloatType>(i) + static_cast<FloatType>(0.5) + random_u) / camera.image_width;
                FloatType v = static_cast<FloatType>(1.0) - (static_cast<FloatType>(j) + static_cast<FloatType>(0.5) + random_v) / camera.image_height; // flip v for image coordinates

                Ray r = camera.get_ray(u, v);
                pixel_color_sum += ray_color(r, max_depth, world);
            }

            pixel_color_sum = pixel_color_sum / static_cast<FloatType>(samples_per_pixel);

            ColorUint8 pixel_color = ColorUint8::from_vec3(pixel_color_sum);

            buffer[3 * (j * camera.image_width + i) + 0] = pixel_color.r;
            buffer[3 * (j * camera.image_width + i) + 1] = pixel_color.g;
            buffer[3 * (j * camera.image_width + i) + 2] = pixel_color.b;
        }
    }
}
