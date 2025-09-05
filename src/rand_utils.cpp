#include "rand_utils.h"

#include <random>

FloatType random_float()
{
    static std::uniform_real_distribution<FloatType> distribution(0.0, 1.0);
    static std::mt19937 generator(std::random_device{}());
    return distribution(generator);
}

FloatType random_float(FloatType min, FloatType max)
{
    static std::uniform_real_distribution<FloatType> distribution(min, max);
    static std::mt19937 generator(std::random_device{}());
    return distribution(generator);
}
