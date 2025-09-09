#pragma once

#include <cstdint>

#include "camera.h"
#include "hittable_list.h"
#include "color.h"

class Renderer
{
public:
    virtual ~Renderer() = default;
    virtual void render(
        const Camera &camera,
        const HittableList &world,
        const Color &background,
        std::uint8_t *buffer) const = 0;
};
