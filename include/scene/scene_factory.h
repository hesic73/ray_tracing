#pragma once

#include <vector>
#include <memory>

#include "hittable_list.h"
#include "material.h"
#include "texture.h"

struct Scene {
    HittableList world;
    std::vector<std::unique_ptr<Texture>> textures;
    std::vector<std::unique_ptr<Material>> materials;
};

enum class SceneType {
    Random = 0,
    TwoSpheres = 1,
    Earth = 2
};

Scene create_scene(SceneType type, FloatType time0, FloatType time1);

