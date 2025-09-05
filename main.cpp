
#include <vector>
#include "spdlog/spdlog.h"
#include "stb_image_write.h"
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/exceptions.h"
#include "vec3.h"
#include "ray.h"
#include "camera.h"

Color3 ray_color(const Ray &r) {
    // For now, return a gradient based on ray direction (simple background)
    Vec3 unit_direction = Vec3::normalize(r.direction);
    double t = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - t) * Color3(1.0, 1.0, 1.0) + t * Color3(0.5, 0.7, 1.0);
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
    try
    {
        image_width = config["image_width"].as<int>();
        image_height = config["image_height"].as<int>();
    }
    catch (const YAML::RepresentationException &e)
    {
        spdlog::error("Error parsing config file: {}", e.what());
        return 1;
    }

    spdlog::info("Image dimensions: {}x{}", image_width, image_height);
    const int channels = 3;

    Camera camera(image_width, image_height);
    camera.set_position(Vec3(0, 0, 0));
    camera.look_at(Vec3(0, 0, -1));

    std::vector<unsigned char> pixels(image_width * image_height * channels);

    for (int j = 0; j < image_height; ++j)
    {
        for (int i = 0; i < image_width; ++i)
        {
            double u = double(i + 0.5) / image_width;
            double v = 1.0 - double(j + 0.5) / image_height; // flip v for image coordinates
            Ray r = camera.get_ray(u, v);
            Color3 pixel_color = ray_color(r);
            pixels[channels * (j * image_width + i) + 0] = static_cast<unsigned char>(255.999 * pixel_color.x);
            pixels[channels * (j * image_width + i) + 1] = static_cast<unsigned char>(255.999 * pixel_color.y);
            pixels[channels * (j * image_width + i) + 2] = static_cast<unsigned char>(255.999 * pixel_color.z);
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