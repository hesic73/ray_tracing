#pragma once

#include <cmath>
#include <limits>

using FloatType = double;

constexpr FloatType infinity_f = std::numeric_limits<FloatType>::infinity();
constexpr FloatType zero_f = static_cast<FloatType>(0.0);
constexpr FloatType one_f = static_cast<FloatType>(1.0);