#pragma once

#include <cstdint>
#include <algorithm>

#include "vec3.h"
#include "common.h"

struct Color
{
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue)
        : r(red), g(green), b(blue) {}
    Color(const Color &c) = default;
    Color &operator=(const Color &c) = default;

    static Color from_float(FloatType red, FloatType green, FloatType blue)
    {
        red = std::clamp(red, static_cast<FloatType>(0.0), static_cast<FloatType>(1.0));
        green = std::clamp(green, static_cast<FloatType>(0.0), static_cast<FloatType>(1.0));
        blue = std::clamp(blue, static_cast<FloatType>(0.0), static_cast<FloatType>(1.0));
        return Color(static_cast<std::uint8_t>(static_cast<FloatType>(255.999) * red),
                     static_cast<std::uint8_t>(static_cast<FloatType>(255.999) * green),
                     static_cast<std::uint8_t>(static_cast<FloatType>(255.999) * blue));
    }
    static Color from_vec3(const Vec3 &v)
    {
        return Color::from_float(v.x, v.y, v.z);
    }

    static Color black() { return Color(0, 0, 0); }
    static Color white() { return Color(255, 255, 255); }
    static Color red() { return Color(255, 0, 0); }
    static Color green() { return Color(0, 255, 0); }
    static Color blue() { return Color(0, 0, 255); }
};