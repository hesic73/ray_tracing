
#include <vector>
#include <memory>
#include <chrono>
#include <string>
#include <sstream>
#include <vector>

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
#include "material.h"

#include "my_renderer.h"

std::string format_duration(std::chrono::milliseconds duration)
{
    auto total_ms = duration.count();

    auto hours = total_ms / (1000 * 60 * 60);
    total_ms %= (1000 * 60 * 60);

    auto minutes = total_ms / (1000 * 60);
    total_ms %= (1000 * 60);

    auto seconds = total_ms / 1000;
    auto milliseconds = total_ms % 1000;

    std::ostringstream oss;
    bool has_prev = false;

    if (hours > 0)
    {
        oss << hours << "h";
        has_prev = true;
    }
    if (minutes > 0)
    {
        if (has_prev)
            oss << " ";
        oss << minutes << "m";
        has_prev = true;
    }
    if (seconds > 0)
    {
        if (has_prev)
            oss << " ";
        oss << seconds << "s";
        has_prev = true;
    }
    if (milliseconds > 0 || !has_prev)
    {
        if (has_prev)
            oss << " ";
        oss << milliseconds << "ms";
    }

    return oss.str();
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
    std::vector<FloatType> camera_position;
    std::vector<FloatType> camera_look_at;
    FloatType focus_dist;
    FloatType defocus_angle;

    int samples_per_pixel;
    int max_depth;

    FloatType gamma;
    try
    {
        fov = config["fov"].as<FloatType>();
        image_width = config["image_width"].as<int>();
        image_height = config["image_height"].as<int>();
        focus_dist = config["focus_dist"].as<FloatType>();
        defocus_angle = config["defocus_angle"].as<FloatType>();

        auto camera_position_node = config["camera_position"];
        for (int i = 0; i < 3; ++i)
        {
            camera_position.push_back(camera_position_node[i].as<FloatType>());
        }

        auto camera_look_at_node = config["camera_look_at"];
        for (int i = 0; i < 3; ++i)
        {
            camera_look_at.push_back(camera_look_at_node[i].as<FloatType>());
        }

        samples_per_pixel = config["samples_per_pixel"].as<int>();
        max_depth = config["max_depth"].as<int>();
        gamma = config["gamma"].as<FloatType>();
    }
    catch (const YAML::RepresentationException &e)
    {
        spdlog::error("Error parsing config file: {}", e.what());
        return 1;
    }

    spdlog::info("Image dimensions: {}x{}", image_width, image_height);

    spdlog::info("Field of view: {}", fov);
    spdlog::info("Camera position: [{}, {}, {}]", camera_position[0], camera_position[1], camera_position[2]);
    spdlog::info("Focus distance: {}", focus_dist);
    spdlog::info("Defocus angle: {}", defocus_angle);
    spdlog::info("Camera look at: [{}, {}, {}]", camera_look_at[0], camera_look_at[1], camera_look_at[2]);

    spdlog::info("Samples per pixel: {}", samples_per_pixel);
    spdlog::info("Max depth: {}", max_depth);

    spdlog::info("Gamma: {}", gamma);

    constexpr int channels = 3;

    HittableList world;

    auto material_ground = Lambertian(ColorFloat(0.8, 0.8, 0.0));
    auto material_center = Lambertian(ColorFloat(0.1, 0.2, 0.5));
    auto material_left = Dielectric(1.50);
    auto material_bubble = Dielectric(1.00 / 1.50);
    auto material_right = Metal(ColorFloat(0.8, 0.6, 0.2), 1.0);

    world.add(std::make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, &material_ground));
    world.add(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5, &material_center));
    world.add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, &material_left));
    world.add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.4, &material_bubble));
    world.add(std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, &material_right));

    Camera camera(image_width, image_height, fov, focus_dist, defocus_angle);
    camera.set_position(Vec3(camera_position[0], camera_position[1], camera_position[2]));
    camera.look_at(Vec3(camera_look_at[0], camera_look_at[1], camera_look_at[2]));

    std::vector<unsigned char> pixels(image_width * image_height * channels);

    std::unique_ptr<Renderer> renderer;
    renderer = std::make_unique<MyRenderer>(samples_per_pixel, max_depth, gamma);

    auto start_time = std::chrono::high_resolution_clock::now();
    renderer->render(camera, world, pixels.data());
    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    spdlog::info("Rendering completed in {}", format_duration(duration));

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