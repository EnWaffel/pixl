#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/core/math/Vec2.h"

namespace px
{
    class Vec2i
	{
	public:
		int x;
		int y;
	public:
        PX_API Vec2i();
		PX_API Vec2i(int x, int y);
		PX_API Vec2i(int v);

		PX_API Vec2i operator+(const Vec2i& other);
		PX_API Vec2i operator+(int v);
		PX_API Vec2i operator-(const Vec2i& other);
		PX_API Vec2i operator-(int v);
		PX_API Vec2i operator*(const Vec2i& other);
		PX_API Vec2i operator*(int v);
		PX_API Vec2i operator/(const Vec2i& other);
		PX_API Vec2i operator/(int v);
		PX_API Vec2i operator^(const Vec2i& other);
		PX_API Vec2i operator^(int v);

		PX_API void operator+=(const Vec2i& other);
		PX_API void operator+=(int v);
		PX_API void operator-=(const Vec2i& other);
		PX_API void operator-=(int v);
		PX_API void operator*=(const Vec2i& other);
		PX_API void operator*=(int v);
		PX_API void operator/=(const Vec2i& other);
		PX_API void operator/=(int v);
		PX_API void operator^=(const Vec2i& other);
		PX_API void operator^=(int v);

		PX_API operator Vec2() const;

		PX_API int Distance(const Vec2i& other);
	};
}