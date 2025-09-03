#pragma once

#include "pixl/utility/Defs.h"

namespace px
{
#pragma pack(push, 1)
    class Vec3
	{
	public:
		float x;
		float y;
		float z;
	public:
		PX_API Vec3();
		PX_API Vec3(float v);
		PX_API Vec3(float x, float y, float z);

		PX_API Vec3 operator+(const Vec3& other);
		PX_API Vec3 operator+(float v);
		PX_API Vec3 operator-(const Vec3& other);
		PX_API Vec3 operator-(float v);
		PX_API Vec3 operator*(const Vec3& other);
		PX_API Vec3 operator*(float v);
		PX_API Vec3 operator/(const Vec3& other);
		PX_API Vec3 operator/(float v);
		PX_API Vec3 operator^(const Vec3& other);
		PX_API Vec3 operator^(float v);

		PX_API void operator+=(const Vec3& other);
		PX_API void operator+=(float v);
		PX_API void operator-=(const Vec3& other);
		PX_API void operator-=(float v);
		PX_API void operator*=(const Vec3& other);
		PX_API void operator*=(float v);
		PX_API void operator/=(const Vec3& other);
		PX_API void operator/=(float v);
		PX_API void operator^=(const Vec3& other);
		PX_API void operator^=(float v);

		PX_API Vec3 operator-();

		PX_API float Dot(const Vec3& other) const;
		PX_API Vec3 Normalize() const;
		PX_API Vec3 Cross(const Vec3& other) const;

		PX_API float Distance(const Vec3& other);
		PX_API float Length();
	};
#pragma pack(pop)
}