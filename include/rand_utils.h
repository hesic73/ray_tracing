#pragma once

#include "common.h"
#include "vec3.h"

FloatType random_float(); // in [0,1)

FloatType random_float(FloatType min, FloatType max); // in [min,max)

Vec3 random_unit_sphere();

Vec3 random_on_hemisphere(const Vec3 &normal);