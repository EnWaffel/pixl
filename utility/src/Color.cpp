#include "pixl/utility/Color.h"

using namespace px;

Color::Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f)
{
}

Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a)
{
}

Color Color::operator+(const Color& other)
{
    return { r + other.r, g + other.g, b + other.b, a + other.a };
}

Color Color::operator+(float v)
{
    return { r + v, g + v, b + v, a + v };
}

Color Color::operator-(const Color& other)
{
    return { r - other.r, g - other.g, b - other.b, a - other.a };
}

Color Color::operator-(float v)
{
    return { r - v, g - v, b - v, a - v };
}

Color Color::operator*(const Color& other)
{
    return { r * other.r, g * other.g, b * other.b, a * other.a };
}

Color Color::operator*(float v)
{
    return { r * v, g * v, b * v, a * v };
}

Color Color::operator/(const Color& other)
{
    return { r / other.r, g / other.g, b / other.b, a / other.a };
}

Color Color::operator/(float v)
{
    return { r / v, g / v, b / v, a / v };
}

void Color::operator+=(const Color& other)
{
    r += other.r;
    g += other.g;
    b += other.b;
    a += other.a;
}

void Color::operator+=(float v)
{
    r += v;
    g += v;
    b += v;
    a += v;
}

void Color::operator-=(const Color& other)
{
    r -= other.r;
    g -= other.g;
    b -= other.b;
    a -= other.a;
}

void Color::operator-=(float v)
{
    r -= v;
    g -= v;
    b -= v;
    a -= v;
}

void Color::operator*=(const Color& other)
{
    r *= other.r;
    g *= other.g;
    b *= other.b;
    a *= other.a;
}

void Color::operator*=(float v)
{
    r *= v;
    g *= v;
    b *= v;
    a *= v;
}

void Color::operator/=(const Color& other)
{
    r /= other.r;
    g /= other.g;
    b /= other.b;
    a /= other.a;
}

void Color::operator/=(float v)
{
    r /= v;
    g /= v;
    b /= v;
    a /= v;
}
