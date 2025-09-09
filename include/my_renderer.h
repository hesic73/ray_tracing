#pragma once

#include "renderer.h"
#include "common.h"

class MyRenderer : public Renderer
{
public:
    MyRenderer(int samples_per_pixel, int max_depth, FloatType gamma)
        : samples_per_pixel(samples_per_pixel), max_depth(max_depth), gamma(gamma) {}
    void render(
        const Camera &camera,
        const HittableList &world,
        const Color &background,
        std::uint8_t *buffer) const override;
private:
    int samples_per_pixel;
    int max_depth;
    FloatType gamma;
};
