#include "perlin.h"

#include <algorithm>
#include <array>
#include <cmath>
static void permute(std::array<int, Perlin::point_count> &p) {
    for (int i = Perlin::point_count - 1; i > 0; --i) {
        int target = static_cast<int>(random_float(0, i + 1));
        std::swap(p[i], p[target]);
    }
}

static FloatType perlin_interp(Vec3 c[2][2][2], FloatType u, FloatType v, FloatType w) {
    auto uu = u * u * (3 - 2 * u);
    auto vv = v * v * (3 - 2 * v);
    auto ww = w * w * (3 - 2 * w);
    FloatType accum = 0;
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k) {
                Vec3 weight(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1 - uu)) *
                         (j * vv + (1 - j) * (1 - vv)) *
                         (k * ww + (1 - k) * (1 - ww)) *
                         Vec3::dot(c[i][j][k], weight);
            }
    return accum;
}

Perlin::Perlin() {
    for (int i = 0; i < point_count; ++i) {
        ranvec[i] = random_unit_sphere();
        perm_x[i] = perm_y[i] = perm_z[i] = i;
    }
    permute(perm_x);
    permute(perm_y);
    permute(perm_z);
}

FloatType Perlin::noise(const Point3 &p) const {
    auto u = p.x - std::floor(p.x);
    auto v = p.y - std::floor(p.y);
    auto w = p.z - std::floor(p.z);
    int i = static_cast<int>(std::floor(p.x));
    int j = static_cast<int>(std::floor(p.y));
    int k = static_cast<int>(std::floor(p.z));
    Vec3 c[2][2][2] = {
        Vec3::uninitialized(), Vec3::uninitialized(),
        Vec3::uninitialized(), Vec3::uninitialized(),
        Vec3::uninitialized(), Vec3::uninitialized(),
        Vec3::uninitialized(), Vec3::uninitialized()};
    for (int di = 0; di < 2; ++di)
        for (int dj = 0; dj < 2; ++dj)
            for (int dk = 0; dk < 2; ++dk) {
                int idx = perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^ perm_z[(k + dk) & 255];
                c[di][dj][dk] = ranvec[idx];
            }
    return perlin_interp(c, u, v, w);
}

FloatType Perlin::turb(const Point3 &p, int depth) const {
    FloatType accum = 0;
    Point3 temp = p;
    FloatType weight = 1;
    for (int i = 0; i < depth; ++i) {
        accum += weight * noise(temp);
        weight *= 0.5;
        temp = temp * 2;
    }
    return std::fabs(accum);
}

