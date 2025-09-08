#include "color.h"

Color operator+(const Color &c1, const Color &c2)
{
    return Color(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b);
}
Color operator*(const Color &c, FloatType t)
{
    return Color(c.r * t, c.g * t, c.b * t);
}
Color operator*(FloatType t, const Color &c)
{
    return Color(c.r * t, c.g * t, c.b * t);
}
Color operator*(const Color &c1, const Color &c2)
{
    return Color(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b);
}
Color operator/(const Color &c, FloatType t)
{
    return Color(c.r / t, c.g / t, c.b / t);
}
