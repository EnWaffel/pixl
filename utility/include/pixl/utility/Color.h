#pragma once

#include "pixl/utility/Defs.h"

namespace px
{
    class Color
	{
	public:
		float r;
		float g;
        float b;
        float a;
	public:
        PX_API Color();
		PX_API Color(float r, float g, float b, float a);

		PX_API Color operator+(const Color& other);
		PX_API Color operator+(float v);
		PX_API Color operator-(const Color& other);
		PX_API Color operator-(float v);
		PX_API Color operator*(const Color& other);
		PX_API Color operator*(float v);
		PX_API Color operator/(const Color& other);
		PX_API Color operator/(float v);

		PX_API void operator+=(const Color& other);
		PX_API void operator+=(float v);
		PX_API void operator-=(const Color& other);
		PX_API void operator-=(float v);
		PX_API void operator*=(const Color& other);
		PX_API void operator*=(float v);
		PX_API void operator/=(const Color& other);
		PX_API void operator/=(float v);
	};
};