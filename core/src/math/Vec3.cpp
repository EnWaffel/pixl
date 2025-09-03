#include "pixl/core/math/Vec3.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace px;

px::Vec3::Vec3() : x(0.0f), y(0.0f), z(0.0f)
{
}

px::Vec3::Vec3(float v) : x(v), y(v), z(v)
{
}

px::Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z)
{
}

Vec3 px::Vec3::operator+(const Vec3& other) {
    return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 px::Vec3::operator+(float v)
{
    return Vec3(x + v, y + v, z + v);
}

Vec3 px::Vec3::operator-(const Vec3& other)
{
    return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 px::Vec3::operator-(float v)
{
    return Vec3(x - v, y - v, z - v);
}

Vec3 px::Vec3::operator*(const Vec3& other)
{
    return Vec3(x * other.x, y * other.y, z * other.z);
}

Vec3 px::Vec3::operator*(float v)
{
    return Vec3(x * v, y * v, z * v);
}

Vec3 px::Vec3::operator/(const Vec3& other)
{
    return Vec3(x / other.x, y / other.y, z / other.z);
}

Vec3 px::Vec3::operator/(float v)
{
    return Vec3(x / v, y / v, z / v);
}

Vec3 px::Vec3::operator^(const Vec3& other)
{
    return Vec3(powf(x, other.x), powf(y, other.y), powf(z, other.z));
}

Vec3 px::Vec3::operator^(float v)
{
    return Vec3(powf(x, v), powf(y, v), powf(z, v));
}

void px::Vec3::operator+=(const Vec3& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
}

void px::Vec3::operator+=(float v)
{
    x += v;
    y += v;
    z += v;
}

void px::Vec3::operator-=(const Vec3& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
}

void px::Vec3::operator-=(float v)
{
    x -= v;
    y -= v;
    z -= v;
}

void px::Vec3::operator*=(const Vec3& other)
{
    x *= other.x;
    y *= other.y;
    z *= other.z;
}

void px::Vec3::operator*=(float v)
{
    x *= v;
    y *= v;
    z *= v;
}

void px::Vec3::operator/=(const Vec3& other)
{
    x /= other.x;
    y /= other.y;
    z /= other.z;
}

void px::Vec3::operator/=(float v)
{
    x /= v;
    y /= v;
    z /= v;
}

void px::Vec3::operator^=(const Vec3& other)
{
    x = powf(x, other.x);
    y = powf(y, other.y);
    z = powf(z, other.z);
}

void px::Vec3::operator^=(float v)
{
    x = powf(x, v);
    y = powf(y, v);
    z = powf(z, v);
}

Vec3 px::Vec3::operator-()
{
    return Vec3(-x, -y, -z);
}

float px::Vec3::Dot(const Vec3& other) const
{
    return x * other.x + y * other.y + z * other.z;
}

Vec3 px::Vec3::Normalize() const
{
    float length = sqrt(x * x + y * y + z * z);
    if (length == 0.0f) return Vec3();
    return Vec3(x / length, y / length, z / length);
}

Vec3 px::Vec3::Cross(const Vec3& other) const
{
    return Vec3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

float px::Vec3::Distance(const Vec3& other)
{
    return sqrtf(powf((other.x - x), 2) + powf(other.y - y, 2));
}

float px::Vec3::Length()
{
    return sqrtf(x * x + y * y + z * z);
}
