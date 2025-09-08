#pragma once

#include <cstdint>
#include <algorithm>
#include <array>

#include "math/vec3.h"
#include "common.h"
#include "rand_utils.h"

struct Color
{
    FloatType r;
    FloatType g;
    FloatType b;
    constexpr Color(FloatType red, FloatType green, FloatType blue)
        : r(red), g(green), b(blue) {}
    Color(const Color &c) = default;
    Color &operator=(const Color &c) = default;

    Vec3 to_vec3() const
    {
        return Vec3(r, g, b);
    }

    static Color black() { return Color(0.0, 0.0, 0.0); }
    static Color white() { return Color(1.0, 1.0, 1.0); }
    static Color red() { return Color(1.0, 0.0, 0.0); }
    static Color green() { return Color(0.0, 1.0, 0.0); }
    static Color blue() { return Color(0.0, 0.0, 1.0); }

    std::array<std::uint8_t, 3> to_uint8() const
    {
        FloatType red = std::clamp(r, static_cast<FloatType>(0.0), static_cast<FloatType>(1.0));
        FloatType green = std::clamp(g, static_cast<FloatType>(0.0), static_cast<FloatType>(1.0));
        FloatType blue = std::clamp(b, static_cast<FloatType>(0.0), static_cast<FloatType>(1.0));
        return {static_cast<std::uint8_t>(static_cast<FloatType>(255.999) * red),
                static_cast<std::uint8_t>(static_cast<FloatType>(255.999) * green),
                static_cast<std::uint8_t>(static_cast<FloatType>(255.999) * blue)};
    }

    static Color pow(const Color &c, FloatType exponent)
    {
        return Color(std::pow(c.r, exponent),
                     std::pow(c.g, exponent),
                     std::pow(c.b, exponent));
    }

    Color &operator+=(const Color &c)
    {
        r += c.r;
        g += c.g;
        b += c.b;
        return *this;
    }

    static Color random()
    {
        return Color(random_float(), random_float(), random_float());
    }

    static Color random(FloatType min, FloatType max)
    {
        return Color(random_float(min, max), random_float(min, max), random_float(min, max));
    }

    static Color uninitialized()
    {
        return Color();
    }

private:
    Color() {}
};

Color operator+(const Color &c1, const Color &c2);
Color operator*(const Color &c, FloatType t);
Color operator*(FloatType t, const Color &c);
Color operator*(const Color &c1, const Color &c2);
Color operator/(const Color &c, FloatType t);

