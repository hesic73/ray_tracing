#pragma once

#include <cstdint>
#include <algorithm>

#include "vec3.h"
#include "common.h"

FloatType random_float();
FloatType random_float(FloatType min, FloatType max);

struct ColorUint8
{
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    constexpr ColorUint8(std::uint8_t red, std::uint8_t green, std::uint8_t blue)
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

struct ColorFloat
{
    FloatType r;
    FloatType g;
    FloatType b;
    constexpr ColorFloat(FloatType red, FloatType green, FloatType blue)
        : r(red), g(green), b(blue) {}
    ColorFloat(const ColorFloat &c) = default;
    ColorFloat &operator=(const ColorFloat &c) = default;

    Vec3 to_vec3() const
    {
        return Vec3(r, g, b);
    }

    static ColorFloat black() { return ColorFloat(0.0, 0.0, 0.0); }
    static ColorFloat white() { return ColorFloat(1.0, 1.0, 1.0); }
    static ColorFloat red() { return ColorFloat(1.0, 0.0, 0.0); }
    static ColorFloat green() { return ColorFloat(0.0, 1.0, 0.0); }
    static ColorFloat blue() { return ColorFloat(0.0, 0.0, 1.0); }

    ColorUint8 to_uint8() const
    {
        return ColorUint8::from_float(r, g, b);
    }

    static ColorFloat pow(const ColorFloat &c, FloatType exponent)
    {
        return ColorFloat(std::pow(c.r, exponent),
                          std::pow(c.g, exponent),
                          std::pow(c.b, exponent));
    }

    ColorFloat &operator+=(const ColorFloat &c)
    {
        r += c.r;
        g += c.g;
        b += c.b;
        return *this;
    }

    static ColorFloat random()
    {
        return ColorFloat(random_float(), random_float(), random_float());
    }

    static ColorFloat random(FloatType min, FloatType max)
    {
        return ColorFloat(random_float(min, max), random_float(min, max), random_float(min, max));
    }

    static ColorFloat uninitialized()
    {
        return ColorFloat();
    }

private:
    ColorFloat() {}
};

ColorFloat operator+(const ColorFloat &c1, const ColorFloat &c2);
ColorFloat operator*(const ColorFloat &c, FloatType t);
ColorFloat operator*(FloatType t, const ColorFloat &c);
ColorFloat operator*(const ColorFloat &c1, const ColorFloat &c2);
ColorFloat operator/(const ColorFloat &c, FloatType t);