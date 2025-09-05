
#include <vector>
#include <memory>

#include "spdlog/spdlog.h"
#include "stb_image_write.h"
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/exceptions.h"

#include "vec3.h"
#include "camera.h"
#include "common.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

#include "my_renderer.h"

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
    int samples_per_pixel;
    FloatType fov;
    try
    {
        image_width = config["image_width"].as<int>();
        image_height = config["image_height"].as<int>();
        fov = config["fov"].as<FloatType>();
        samples_per_pixel = config["samples_per_pixel"].as<int>();
    }
    catch (const YAML::RepresentationException &e)
    {
        spdlog::error("Error parsing config file: {}", e.what());
        return 1;
    }

    spdlog::info("Image dimensions: {}x{}", image_width, image_height);
    spdlog::info("Field of view: {}", fov);
    spdlog::info("Samples per pixel: {}", samples_per_pixel);

    constexpr int channels = 3;

    HittableList world;
    world.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    Camera camera(image_width, image_height, fov);
    camera.set_position(Vec3(0, 0, 0));
    camera.look_at(Vec3(0, 0, -1));

    std::vector<unsigned char> pixels(image_width * image_height * channels);

    std::unique_ptr<Renderer> renderer;
    renderer = std::make_unique<MyRenderer>(samples_per_pixel);
    renderer->render(camera, world, pixels.data());

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