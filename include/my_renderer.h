#pragma once

#include "renderer.h"

class MyRenderer : public Renderer
{
public:
    MyRenderer(int samples_per_pixel, int max_depth);
    void render(
        const Camera &camera,
        const Hittable &world,
        std::uint8_t *buffer) const override;
private:
    int samples_per_pixel;
    int max_depth;
};
