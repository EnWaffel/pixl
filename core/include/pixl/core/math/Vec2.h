#pragma once

#include "pixl/utility/Defs.h"

namespace px
{
	class Vec2i;
#pragma pack(push, 1)
    class Vec2
	{
	public:
		float x;
		float y;
	public:
        PX_API Vec2();
		PX_API Vec2(float x, float y);
		PX_API Vec2(float v);
		PX_API Vec2(const Vec2i& vec);

		PX_API Vec2 operator+(const Vec2& other);
		PX_API Vec2 operator+(float v);
		PX_API Vec2 operator-(const Vec2& other);
		PX_API Vec2 operator-(float v);
		PX_API Vec2 operator*(const Vec2& other);
		PX_API Vec2 operator*(float v);
		PX_API Vec2 operator/(const Vec2& other);
		PX_API Vec2 operator/(float v);
		PX_API Vec2 operator^(const Vec2& other);
		PX_API Vec2 operator^(float v);

		PX_API void operator+=(const Vec2& other);
		PX_API void operator+=(float v);
		PX_API void operator-=(const Vec2& other);
		PX_API void operator-=(float v);
		PX_API void operator*=(const Vec2& other);
		PX_API void operator*=(float v);
		PX_API void operator/=(const Vec2& other);
		PX_API void operator/=(float v);
		PX_API void operator^=(const Vec2& other);
		PX_API void operator^=(float v);

		PX_API Vec2 operator-();

		PX_API float Distance(const Vec2& other);
	};
#pragma pack(pop)
}