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
};
