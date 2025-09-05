#pragma once

#include <cstdint>
#include <memory>

#include "camera.h"
#include "hittable.h"

class Renderer
{
public:
    virtual ~Renderer() = default;
    virtual void render(
        const Camera &camera,
        const Hittable &world,
        std::uint8_t *buffer) const = 0;
};
