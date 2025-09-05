
#include <vector>
#include "spdlog/spdlog.h"
#include "stb_image_write.h"
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/exceptions.h"

#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "camera.h"
#include "common.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

Color ray_color(const Ray &r, const Hittable &world)
{
    HitRecord hit_record = HitRecord::default_record();
    if (world.hit(r, zero_f, infinity_f, hit_record))
    {
        Vec3 N = hit_record.normal;
        return Color::from_float(static_cast<FloatType>(0.5) * (N.x + static_cast<FloatType>(1.0)),
                                 static_cast<FloatType>(0.5) * (N.y + static_cast<FloatType>(1.0)),
                                 static_cast<FloatType>(0.5) * (N.z + static_cast<FloatType>(1.0)));
    }

    Vec3 unit_direction = Vec3::normalize(r.direction);
    auto a = static_cast<FloatType>(0.5) * (unit_direction.y + static_cast<FloatType>(1.0));
    return Color::from_vec3((static_cast<FloatType>(1.0) - a) * Vec3(static_cast<FloatType>(1.0), static_cast<FloatType>(1.0), static_cast<FloatType>(1.0)) + a * Vec3(static_cast<FloatType>(0.5), static_cast<FloatType>(0.7), static_cast<FloatType>(1.0)));
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        spdlog::error("Usage: {} <config.yaml>", argv[0]);
        return 0;
    }

    const char *config_file = argv[1];
    YAML::Node config;
    try
    {
        config = YAML::LoadFile(config_file);
    }
    catch (const YAML::BadFile &e)
    {
        spdlog::error("Failed to load config file {}: {}", config_file, e.what());
        return 1;
    }

    int image_width;
    int image_height;
    FloatType fov;
    try
    {
        image_width = config["image_width"].as<int>();
        image_height = config["image_height"].as<int>();
        fov = config["fov"].as<FloatType>();
    }
    catch (const YAML::RepresentationException &e)
    {
        spdlog::error("Error parsing config file: {}", e.what());
        return 1;
    }

    spdlog::info("Image dimensions: {}x{}", image_width, image_height);
    spdlog::info("Field of view: {}", fov);

    constexpr int channels = 3;

    HittableList world;
    world.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    Camera camera(image_width, image_height, fov);
    camera.set_position(Vec3(0, 0, 0));
    camera.look_at(Vec3(0, 0, -1));

    std::vector<unsigned char> pixels(image_width * image_height * channels);

    for (int j = 0; j < image_height; ++j)
    {
        for (int i = 0; i < image_width; ++i)
        {
            FloatType u = static_cast<FloatType>(i + 0.5) / image_width;
            FloatType v = static_cast<FloatType>(1.0) - static_cast<FloatType>(j + 0.5) / image_height; // flip v for image coordinates
            Ray r = camera.get_ray(u, v);
            Color pixel_color = ray_color(r, world);
            pixels[channels * (j * image_width + i) + 0] = static_cast<unsigned char>(pixel_color.r);
            pixels[channels * (j * image_width + i) + 1] = static_cast<unsigned char>(pixel_color.g);
            pixels[channels * (j * image_width + i) + 2] = static_cast<unsigned char>(pixel_color.b);
        }
    }

    const char *filename = "output.png";
    int stride_in_bytes = image_width * channels;

    if (stbi_write_png(filename, image_width, image_height, channels, pixels.data(), stride_in_bytes))
    {
        spdlog::info("Successfully saved image to {}", filename);
    }
    else
    {
        spdlog::error("Failed to save image to {}", filename);
        return 1;
    }

    return 0;
}