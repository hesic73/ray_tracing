#pragma once

#include <array>

#include "math/vec3.h"
#include "rand_utils.h"

struct Perlin {
    static constexpr int point_count = 256;

    Perlin();

    FloatType noise(const Point3 &p) const;
    FloatType turb(const Point3 &p, int depth = 7) const;

    std::array<Vec3, point_count> ranvec;
    std::array<int, point_count> perm_x;
    std::array<int, point_count> perm_y;
    std::array<int, point_count> perm_z;
};

