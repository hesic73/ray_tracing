#include "color.h"

ColorFloat operator+(const ColorFloat &c1, const ColorFloat &c2)
{
    return ColorFloat(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b);
}
ColorFloat operator*(const ColorFloat &c, FloatType t)
{
    return ColorFloat(c.r * t, c.g * t, c.b * t);
}
ColorFloat operator*(FloatType t, const ColorFloat &c)
{
    return ColorFloat(c.r * t, c.g * t, c.b * t);
}
ColorFloat operator*(const ColorFloat &c1, const ColorFloat &c2)
{
    return ColorFloat(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b);
}
ColorFloat operator/(const ColorFloat &c, FloatType t)
{
    return ColorFloat(c.r / t, c.g / t, c.b / t);
}