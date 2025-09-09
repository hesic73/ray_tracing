#include "my_renderer.h"

#include "math/vec3.h"
#include "ray.h"
#include "color.h"
#include "camera.h"
#include "common.h"
#include "rand_utils.h"

#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "bvh.h"

#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/partitioner.h>

Color ray_color(const Ray &r, int depth, const Hittable &world, const Color &background)
{
    if (depth <= 0)
        return Color::black();
    HitRecord hit_record = HitRecord::uninitialized();
    if (!world.hit(r, Interval(static_cast<FloatType>(0.001), infinity_f), hit_record))
    {
        return background;
    }

    Ray scattered = Ray::uninitialized();
    Color attenuation = Color::uninitialized();
    Color emitted = hit_record.material ? hit_record.material->emitted(hit_record.u, hit_record.v, hit_record.point) : Color::black();
    if (!hit_record.material || !hit_record.material->scatter(r, hit_record, attenuation, scattered))
    {
        return emitted;
    }
    return emitted + attenuation * ray_color(scattered, depth - 1, world, background);
}

void MyRenderer::render(
    const Camera &camera,
    const HittableList &world,
    const Color &background,
    std::uint8_t *buffer) const
{
    auto objects = world.objects;
    BVH bvh(objects, camera.time1);

    tbb::parallel_for(
        tbb::blocked_range<int>(0, camera.image_height, 8),
        [&](const tbb::blocked_range<int>& rows) {
            for (int j = rows.begin(); j != rows.end(); ++j) {
                for (int i = 0; i < camera.image_width; ++i) {
                    Color pixel_color_sum = Color::black();

                    for (int sample = 0; sample < samples_per_pixel; ++sample)
                    {
                        FloatType random_u = random_float() - static_cast<FloatType>(0.5); // [-0.5, 0.5)
                        FloatType random_v = random_float() - static_cast<FloatType>(0.5); // [-0.5, 0.5)

                        FloatType u = (static_cast<FloatType>(i) + static_cast<FloatType>(0.5) + random_u) / camera.image_width;
                        FloatType v = static_cast<FloatType>(1.0) - (static_cast<FloatType>(j) + static_cast<FloatType>(0.5) + random_v) / camera.image_height; // flip v for image coordinates

                        Ray r = camera.get_ray(u, v);
                        pixel_color_sum += ray_color(r, max_depth, bvh, background);
                    }

                    pixel_color_sum = pixel_color_sum / static_cast<FloatType>(samples_per_pixel);

                    // Gamma correction
                    pixel_color_sum = Color::pow(pixel_color_sum, gamma);

                    auto pixel_color = pixel_color_sum.to_uint8();

                    buffer[3 * (j * camera.image_width + i) + 0] = pixel_color[0];
                    buffer[3 * (j * camera.image_width + i) + 1] = pixel_color[1];
                    buffer[3 * (j * camera.image_width + i) + 2] = pixel_color[2];
                }
            }
        },
        tbb::static_partitioner()
    );
}
