#include "pixl/core/math/Vec2i.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace px;

px::Vec2i::Vec2i() : x(0), y(0)
{
}

px::Vec2i::Vec2i(int x, int y) : x(x), y(y)
{
}

px::Vec2i::Vec2i(int v) : x(v), y(v)
{
}

Vec2i px::Vec2i::operator+(const Vec2i &other)
{
    return Vec2i(x + other.x, y + other.y);
}

Vec2i px::Vec2i::operator+(int v)
{
    return Vec2i(x + v, y + v);
}

Vec2i px::Vec2i::operator-(const Vec2i& other)
{
    return Vec2i(x - other.x, y - other.y);
}

Vec2i px::Vec2i::operator-(int v)
{
    return Vec2i(x - v, y - v);
}

Vec2i px::Vec2i::operator*(const Vec2i& other)
{
    return Vec2i(x * other.x, y * other.y);
}

Vec2i px::Vec2i::operator*(int v)
{
    return Vec2i(x * v, y * v);
}

Vec2i px::Vec2i::operator/(const Vec2i& other)
{
    return Vec2i(x / other.x, y / other.y);
}

Vec2i px::Vec2i::operator/(int v)
{
    return Vec2i(x / v, y / v);
}

Vec2i px::Vec2i::operator^(const Vec2i& other)
{
    return Vec2i(powf(x, other.x), powf(y, other.y));
}

Vec2i px::Vec2i::operator^(int v)
{
    return Vec2i(powf(x, v), powf(y, v));
}

void px::Vec2i::operator+=(const Vec2i& other)
{
    x += other.x;
    y += other.y;
}

void px::Vec2i::operator+=(int v)
{
    x += v;
    y += v;
}

void px::Vec2i::operator-=(const Vec2i& other)
{
    x -= other.x;
    y -= other.y;
}

void px::Vec2i::operator-=(int v)
{
    x -= v;
    y -= v;
}

void px::Vec2i::operator*=(const Vec2i& other)
{
    x *= other.x;
    y *= other.y;
}

void px::Vec2i::operator*=(int v)
{
    x *= v;
    y *= v;
}

void px::Vec2i::operator/=(const Vec2i& other)
{
    x /= other.x;
    y /= other.y;
}

void px::Vec2i::operator/=(int v)
{
    x /= v;
    y /= v;
}

void px::Vec2i::operator^=(const Vec2i& other)
{
    x = powf(x, other.x);
    y = powf(y, other.y);
}

void px::Vec2i::operator^=(int v)
{
    x = powf(x, v);
    y = powf(y, v);
}

px::Vec2i::operator Vec2() const
{
    return Vec2(x, y);
}

int px::Vec2i::Distance(const Vec2i &other)
{
    return sqrtf(powf((other.x - x), 2) + powf(other.y - y, 2));
}