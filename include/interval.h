#pragma once

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

    static Interval interval_zero_infinity()
    {
        return Interval(static_cast<FloatType>(0), infinity_f);
    }
};
