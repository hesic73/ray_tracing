#pragma once

#include <cstdint>
#include <algorithm>

#include "vec3.h"
#include "common.h"

struct ColorUint8
{
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    ColorUint8(std::uint8_t red, std::uint8_t green, std::uint8_t blue)
        : r(red), g(green), b(blue) {}
    ColorUint8(const ColorUint8 &c) = default;
    ColorUint8 &operator=(const ColorUint8 &c) = default;

    static ColorUint8 from_float(FloatType red, FloatType green, FloatType blue)
    {
        red = std::clamp(red, static_cast<FloatType>(0.0), static_cast<FloatType>(1.0));
        green = std::clamp(green, static_cast<FloatType>(0.0), static_cast<FloatType>(1.0));
        blue = std::clamp(blue, static_cast<FloatType>(0.0), static_cast<FloatType>(1.0));
        return ColorUint8(static_cast<std::uint8_t>(static_cast<FloatType>(255.999) * red),
                     static_cast<std::uint8_t>(static_cast<FloatType>(255.999) * green),
                     static_cast<std::uint8_t>(static_cast<FloatType>(255.999) * blue));
    }
    static ColorUint8 from_vec3(const Vec3 &v)
    {
        return ColorUint8::from_float(v.x, v.y, v.z);
    }

    static ColorUint8 black() { return ColorUint8(0, 0, 0); }
    static ColorUint8 white() { return ColorUint8(255, 255, 255); }
    static ColorUint8 red() { return ColorUint8(255, 0, 0); }
    static ColorUint8 green() { return ColorUint8(0, 255, 0); }
    static ColorUint8 blue() { return ColorUint8(0, 0, 255); }
};