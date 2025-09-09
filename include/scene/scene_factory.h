#pragma once

#include <vector>
#include <memory>
#include <string>

#include "hittable_list.h"
#include "material.h"
#include "texture.h"
#include "color.h"

struct Scene {
    HittableList world;
    std::vector<std::unique_ptr<Texture>> textures;
    std::vector<std::unique_ptr<Material>> materials;
    Color background = Color::black();
};

std::unique_ptr<Scene> create_scene(const std::string &type, FloatType time0, FloatType time1);

