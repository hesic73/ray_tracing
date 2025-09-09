#include "scene/scene_factory.h"
#include "primitives/sphere.h"
#include "primitives/quad.h"
#include "primitives/triangle.h"
#include "texture.h"
#include "rand_utils.h"

static std::unique_ptr<Scene> random_scene(FloatType time0, FloatType time1)
{
    auto scene = std::make_unique<Scene>();
    auto ground_tex = std::make_unique<SolidColorTexture>(Color(0.5, 0.5, 0.5));
    const Texture *ground_ptr = ground_tex.get();
    scene->textures.push_back(std::move(ground_tex));
    auto ground = std::make_unique<Lambertian>(ground_ptr);
    scene->world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground.get(), Motion(), time0));
    scene->materials.push_back(std::move(ground));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = random_float();
            Point3 center(a + 0.9 * random_float(), 0.2, b + 0.9 * random_float());
            if ((center - Point3(4, 0.2, 0)).norm() > 0.9)
            {
                if (choose_mat < 0.8)
                {
                    auto albedo = Color::random() * Color::random();
                    auto tex = std::make_unique<SolidColorTexture>(albedo);
                    const Texture *tex_ptr = tex.get();
                    scene->textures.push_back(std::move(tex));
                    auto mat = std::make_unique<Lambertian>(tex_ptr);
                    auto motion = Motion(Vec3(0, random_float(0, 0.5), 0));
                    scene->world.add(std::make_shared<Sphere>(center, 0.2, mat.get(), motion, time0));
                    scene->materials.push_back(std::move(mat));
                }
                else if (choose_mat < 0.95)
                {
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = random_float(0, 0.5);
                    auto mat = std::make_unique<Metal>(albedo, fuzz);
                    scene->world.add(std::make_shared<Sphere>(center, 0.2, mat.get(), Motion(), time0));
                    scene->materials.push_back(std::move(mat));
                }
                else
                {
                    auto mat = std::make_unique<Dielectric>(1.5);
                    scene->world.add(std::make_shared<Sphere>(center, 0.2, mat.get(), Motion(), time0));
                    scene->materials.push_back(std::move(mat));
                }
            }
        }
    }

    auto mat1 = std::make_unique<Dielectric>(1.5);
    scene->world.add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, mat1.get(), Motion(), time0));
    scene->materials.push_back(std::move(mat1));

    auto tex2 = std::make_unique<SolidColorTexture>(Color(0.4, 0.2, 0.1));
    const Texture *tex2_ptr = tex2.get();
    scene->textures.push_back(std::move(tex2));
    auto mat2 = std::make_unique<Lambertian>(tex2_ptr);
    scene->world.add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, mat2.get(), Motion(), time0));
    scene->materials.push_back(std::move(mat2));

    auto mat3 = std::make_unique<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    scene->world.add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, mat3.get(), Motion(), time0));
    scene->materials.push_back(std::move(mat3));

    return scene;
}

static std::unique_ptr<Scene> two_spheres_scene(FloatType time0, FloatType time1)
{
    auto scene = std::make_unique<Scene>();
    auto checker = std::make_unique<CheckerTexture>(
        std::make_unique<SolidColorTexture>(Color(0.2, 0.3, 0.1)),
        std::make_unique<SolidColorTexture>(Color(0.9, 0.9, 0.9)),
        10.0);
    const Texture *checker_ptr = checker.get();
    scene->textures.push_back(std::move(checker));
    auto mat = std::make_unique<Lambertian>(checker_ptr);
    scene->world.add(std::make_shared<Sphere>(Point3(0, -10, 0), 10, mat.get(), Motion(), time0));
    scene->world.add(std::make_shared<Sphere>(Point3(0, 10, 0), 10, mat.get(), Motion(), time0));
    scene->materials.push_back(std::move(mat));
    return scene;
}

static std::unique_ptr<Scene> earth_scene(FloatType time0, FloatType time1)
{
    auto scene = std::make_unique<Scene>();
    auto earth = std::make_unique<ImageTexture>("assets/textures/earthmap.jpg");
    const Texture *earth_ptr = earth.get();
    scene->textures.push_back(std::move(earth));
    auto mat = std::make_unique<Lambertian>(earth_ptr);
    scene->world.add(std::make_shared<Sphere>(Point3(0, 0, 0), 2, mat.get(), Motion(), time0));
    scene->materials.push_back(std::move(mat));
    return scene;
}

static std::unique_ptr<Scene> perlin_scene(FloatType time0, FloatType time1)
{
    auto scene = std::make_unique<Scene>();
    auto pertext = std::make_unique<NoiseTexture>(4.0);
    const Texture *pertext_ptr = pertext.get();
    scene->textures.push_back(std::move(pertext));
    auto mat = std::make_unique<Lambertian>(pertext_ptr);
    scene->world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, mat.get(), Motion(), time0));
    scene->world.add(std::make_shared<Sphere>(Point3(0, 2, 0), 2, mat.get(), Motion(), time0));
    scene->materials.push_back(std::move(mat));
    return scene;
}

static std::unique_ptr<Scene> quads_scene(FloatType time0, FloatType time1)
{
    auto scene = std::make_unique<Scene>();

    {
        auto tex = std::make_unique<SolidColorTexture>(Color(1.0, 0.2, 0.2));
        const Texture *tex_ptr = tex.get();
        scene->textures.push_back(std::move(tex));
        auto mat = std::make_unique<Lambertian>(tex_ptr);
        scene->world.add(std::make_shared<Quad>(Point3(-3, -2, 5), Vec3(0, 0, -4), Vec3(0, 4, 0), mat.get()));
        scene->materials.push_back(std::move(mat));
    }

    {
        auto tex = std::make_unique<SolidColorTexture>(Color(0.2, 1.0, 0.2));
        const Texture *tex_ptr = tex.get();
        scene->textures.push_back(std::move(tex));
        auto mat = std::make_unique<Lambertian>(tex_ptr);
        scene->world.add(std::make_shared<Quad>(Point3(-2, -2, 0), Vec3(4, 0, 0), Vec3(0, 4, 0), mat.get()));
        scene->materials.push_back(std::move(mat));
    }

    {
        auto tex = std::make_unique<SolidColorTexture>(Color(0.2, 0.2, 1.0));
        const Texture *tex_ptr = tex.get();
        scene->textures.push_back(std::move(tex));
        auto mat = std::make_unique<Lambertian>(tex_ptr);
        scene->world.add(std::make_shared<Quad>(Point3(3, -2, 1), Vec3(0, 0, 4), Vec3(0, 4, 0), mat.get()));
        scene->materials.push_back(std::move(mat));
    }

    {
        auto tex = std::make_unique<SolidColorTexture>(Color(1.0, 0.5, 0.0));
        const Texture *tex_ptr = tex.get();
        scene->textures.push_back(std::move(tex));
        auto mat = std::make_unique<Lambertian>(tex_ptr);
        scene->world.add(std::make_shared<Quad>(Point3(-2, 3, 1), Vec3(4, 0, 0), Vec3(0, 0, 4), mat.get()));
        scene->materials.push_back(std::move(mat));
    }

    {
        auto tex = std::make_unique<SolidColorTexture>(Color(0.2, 0.8, 0.8));
        const Texture *tex_ptr = tex.get();
        scene->textures.push_back(std::move(tex));
        auto mat = std::make_unique<Lambertian>(tex_ptr);
        scene->world.add(std::make_shared<Quad>(Point3(-2, -3, 5), Vec3(4, 0, 0), Vec3(0, 0, -4), mat.get()));
        scene->materials.push_back(std::move(mat));
    }

    return scene;
}

static std::unique_ptr<Scene> triangles_scene(FloatType time0, FloatType time1)
{
    auto scene = std::make_unique<Scene>();

    {
        auto ground_tex = std::make_unique<SolidColorTexture>(Color(0.8, 0.8, 0.8));
        const Texture *ground_ptr = ground_tex.get();
        scene->textures.push_back(std::move(ground_tex));
        auto ground_mat = std::make_unique<Lambertian>(ground_ptr);
        scene->world.add(std::make_shared<Triangle>(Point3(-4, -2, 0), Point3(4, -2, 0), Point3(4, -2, 6), ground_mat.get()));
        scene->world.add(std::make_shared<Triangle>(Point3(-4, -2, 0), Point3(4, -2, 6), Point3(-4, -2, 6), ground_mat.get()));
        scene->materials.push_back(std::move(ground_mat));
    }

    {
        auto metal_mat = std::make_unique<Metal>(Color(0.8, 0.8, 0.9), 0.1);
        Point3 a(-1, -2, 1), b(1, -2, 1), c(1, -2, 3), d(-1, -2, 3);
        Point3 apex(0, 0, 2);
        scene->world.add(std::make_shared<Triangle>(a, b, apex, metal_mat.get()));
        scene->world.add(std::make_shared<Triangle>(b, c, apex, metal_mat.get()));
        scene->world.add(std::make_shared<Triangle>(c, d, apex, metal_mat.get()));
        scene->world.add(std::make_shared<Triangle>(d, a, apex, metal_mat.get()));
        scene->materials.push_back(std::move(metal_mat));
    }

    {
        auto wall_tex = std::make_unique<SolidColorTexture>(Color(0.2, 0.7, 0.4));
        const Texture *wall_ptr = wall_tex.get();
        scene->textures.push_back(std::move(wall_tex));
        auto wall_mat = std::make_unique<Lambertian>(wall_ptr);
        scene->world.add(std::make_shared<Triangle>(Point3(-3, -1, 5), Point3(-1, 2, 5.5), Point3(-3, 2, 3.5), wall_mat.get()));
        scene->materials.push_back(std::move(wall_mat));
    }

    {
        auto glass_mat = std::make_unique<Dielectric>(1.5);
        scene->world.add(std::make_shared<Triangle>(Point3(2.5, -1.5, 4.5), Point3(3.5, 1.5, 3.0), Point3(1.5, 1.0, 4.0), glass_mat.get()));
        scene->materials.push_back(std::move(glass_mat));
    }

    return scene;
}

std::unique_ptr<Scene> create_scene(const std::string &type, FloatType time0, FloatType time1)
{
    if (type == "random")
        return random_scene(time0, time1);
    if (type == "two_spheres")
        return two_spheres_scene(time0, time1);
    if (type == "earth")
        return earth_scene(time0, time1);
    if (type == "perlin")
        return perlin_scene(time0, time1);
    if (type == "quads")
        return quads_scene(time0, time1);
    if (type == "triangles")
        return triangles_scene(time0, time1);
    return std::make_unique<Scene>();
}
