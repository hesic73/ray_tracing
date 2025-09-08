#pragma once

#include <cstdint>

#include "camera.h"
#include "hittable_list.h"

class Renderer
{
public:
    virtual ~Renderer() = default;
    virtual void render(
        const Camera &camera,
        const HittableList &world,
        std::uint8_t *buffer) const = 0;
};
