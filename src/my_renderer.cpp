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
#include <atomic>
#include <cmath>
#include "indicators/indicators.hpp"

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

    indicators::ProgressBar progress_bar{
        indicators::option::BarWidth{50},
        indicators::option::Start{"["},
        indicators::option::Fill{"="},
        indicators::option::Lead{">"},
        indicators::option::Remainder{"-"},
        indicators::option::End{"]"},
        indicators::option::PrefixText{"Rendering "},
        indicators::option::PostfixText{" Complete"},
        indicators::option::ForegroundColor{indicators::Color::green},
        indicators::option::ShowPercentage{true},
        indicators::option::ShowElapsedTime{true},
        indicators::option::ShowRemainingTime{true},
        indicators::option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};

    std::atomic<int> completed_rows{0};
    const int total_rows = camera.image_height;


    const int sqrt_spp = static_cast<int>(std::sqrt(samples_per_pixel));
    const int actual_samples = sqrt_spp * sqrt_spp;
    const int remaining_samples = samples_per_pixel - actual_samples;
    const FloatType inv_sqrt_spp = static_cast<FloatType>(1.0) / static_cast<FloatType>(sqrt_spp);
    const FloatType inv_image_width = static_cast<FloatType>(1.0) / camera.image_width;
    const FloatType inv_image_height = static_cast<FloatType>(1.0) / camera.image_height;
    const FloatType inv_samples_per_pixel = static_cast<FloatType>(1.0) / static_cast<FloatType>(samples_per_pixel);

    tbb::parallel_for(
        tbb::blocked_range<int>(0, camera.image_height, 8),
        [&](const tbb::blocked_range<int> &rows)
        {
            for (int j = rows.begin(); j != rows.end(); ++j)
            {
                // Pre-calculate pixel row coordinate
                const FloatType pixel_v_base = static_cast<FloatType>(1.0) - (static_cast<FloatType>(j) + static_cast<FloatType>(0.5)) * inv_image_height;
                
                for (int i = 0; i < camera.image_width; ++i)
                {
                    Color pixel_color_sum = Color::black();

                    // Pre-calculate pixel column coordinate
                    const FloatType pixel_u_base = (static_cast<FloatType>(i) + static_cast<FloatType>(0.5)) * inv_image_width;
                    
                    // Sample from each stratum
                    for (int stratum_j = 0; stratum_j < sqrt_spp; ++stratum_j)
                    {
                        const FloatType stratum_j_offset = static_cast<FloatType>(stratum_j) * inv_sqrt_spp - static_cast<FloatType>(0.5);
                        
                        for (int stratum_i = 0; stratum_i < sqrt_spp; ++stratum_i)
                        {
                            const FloatType stratum_i_offset = static_cast<FloatType>(stratum_i) * inv_sqrt_spp - static_cast<FloatType>(0.5);
                            
                            FloatType random_u = random_float();
                            FloatType random_v = random_float();
                            FloatType stratum_u = stratum_i_offset + random_u * inv_sqrt_spp;
                            FloatType stratum_v = stratum_j_offset + random_v * inv_sqrt_spp;

                            FloatType u = pixel_u_base + stratum_u * inv_image_width;
                            FloatType v = pixel_v_base - stratum_v * inv_image_height; // flip v for image coordinates

                            Ray r = camera.get_ray(u, v);
                            pixel_color_sum += ray_color(r, max_depth, bvh, background);
                        }
                    }
                    
                    // Handle remaining samples if samples_per_pixel is not a perfect square
                    for (int sample = 0; sample < remaining_samples; ++sample)
                    {
                        FloatType random_u = random_float() - static_cast<FloatType>(0.5);
                        FloatType random_v = random_float() - static_cast<FloatType>(0.5);

                        FloatType u = pixel_u_base + random_u * inv_image_width;
                        FloatType v = pixel_v_base - random_v * inv_image_height; // flip v for image coordinates

                        Ray r = camera.get_ray(u, v);
                        pixel_color_sum += ray_color(r, max_depth, bvh, background);
                    }

                    pixel_color_sum = pixel_color_sum * inv_samples_per_pixel;

                    // Gamma correction
                    pixel_color_sum = Color::pow(pixel_color_sum, gamma);

                    auto pixel_color = pixel_color_sum.to_uint8();

                    buffer[3 * (j * camera.image_width + i) + 0] = pixel_color[0];
                    buffer[3 * (j * camera.image_width + i) + 1] = pixel_color[1];
                    buffer[3 * (j * camera.image_width + i) + 2] = pixel_color[2];
                }

                int current_completed = completed_rows.fetch_add(1) + 1;
                float progress = static_cast<float>(current_completed) / static_cast<float>(total_rows);
                progress_bar.set_progress(static_cast<size_t>(progress * 100));
            }
        },
        tbb::static_partitioner());

    progress_bar.mark_as_completed();
}
