#pragma once

#include <memory>
#include <string>
#include <cmath>

#include "color.h"
#include "math/vec3.h"
#include "perlin.h"

struct Texture {
    virtual ~Texture() = default;
    virtual Color value(FloatType u, FloatType v, const Point3 &p) const = 0;
};

struct SolidColorTexture : public Texture {
    SolidColorTexture(const Color &c) : color_value(c) {}
    Color value(FloatType u, FloatType v, const Point3 &p) const override {
        return color_value;
    }
    Color color_value;
};

struct CheckerTexture : public Texture {
    CheckerTexture(std::unique_ptr<Texture> even, std::unique_ptr<Texture> odd, FloatType scale = 10.0)
        : even(std::move(even)), odd(std::move(odd)), scale(scale) {}
    Color value(FloatType u, FloatType v, const Point3 &p) const override {
        auto s = std::sin(scale * p.x) * std::sin(scale * p.y) * std::sin(scale * p.z);
        return s < 0 ? odd->value(u, v, p) : even->value(u, v, p);
    }
    std::unique_ptr<Texture> even;
    std::unique_ptr<Texture> odd;
    FloatType scale;
};

struct ImageTexture : public Texture {
    ImageTexture(const std::string &filename);
    ~ImageTexture();
    Color value(FloatType u, FloatType v, const Point3 &p) const override;

    unsigned char *data = nullptr;
    int width = 0;
    int height = 0;
    int bytes_per_scanline = 0;
};

struct NoiseTexture : public Texture {
    NoiseTexture(FloatType scale = 1.0) : scale(scale) {}
    Color value(FloatType u, FloatType v, const Point3 &p) const override {
        return Color(1, 1, 1) * 0.5 * (1 + std::sin(scale * p.z + 10 * noise.turb(p)));
    }
    Perlin noise;
    FloatType scale;
};

