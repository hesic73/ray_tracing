#pragma once

#include <algorithm>

#include "common.h"

struct Interval
{
    FloatType min;
    FloatType max;

    Interval(FloatType min, FloatType max)
        : min(min), max(max) {}

    bool contains(FloatType value) const
    {
        return value >= min && value <= max;
    }

    bool surrounds(FloatType value) const
    {
        return value > min && value < max;
    }

    FloatType clamp(FloatType value) const
    {
        return std::clamp(value, min, max);
    }

    Interval expand(FloatType delta) const
    {
        FloatType pad = delta / static_cast<FloatType>(2.0);
        return Interval(min - pad, max + pad);
    }

    static Interval empty()
    {
        return Interval(infinity_f, -infinity_f);
    }
};
