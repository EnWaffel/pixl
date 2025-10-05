#pragma once

#include "pixl/utility/Defs.h"

#define PX_TRANSPARENT px::Color(0.0f, 0.0f, 0.0f, 0.0f)
#define PX_BLACK px::Color(0.0f, 0.0f, 0.0f, 1.0f)
#define PX_WHITE px::Color(1.0f, 1.0f, 1.0f, 1.0f)
#define PX_RED px::Color(1.0f, 0.0f, 0.0f, 1.0f)
#define PX_GREEN px::Color(0.0f, 1.0f, 0.0f, 1.0f)
#define PX_BLUE px::Color(0.0f, 0.0f, 1.0f, 1.0f)
#define PX_YELLOW px::Color(1.0f, 1.0f, 0.0f, 1.0f)
#define PX_CYAN px::Color(0.0f, 1.0f, 1.0f, 1.0f)
#define PX_MAGENTA px::Color(1.0f, 0.0f, 1.0f, 1.0f)
#define PX_ORANGE px::Color(1.0f, 0.5f, 0.0f, 1.0f)
#define PX_PURPLE px::Color(0.5f, 0.0f, 0.5f, 1.0f)
#define PX_GRAY px::Color(0.5f, 0.5f, 0.5f, 1.0f)
#define PX_LIGHT_GRAY px::Color(0.75f, 0.75f, 0.75f, 1.0f)
#define PX_DARK_GRAY px::Color(0.25f, 0.25f, 0.25f, 1.0f)
#define PX_BROWN px::Color(0.6f, 0.3f, 0.0f, 1.0f)
#define PX_PINK px::Color(1.0f, 0.75f, 0.8f, 1.0f)
#define PX_MAROON px::Color(0.5f, 0.0f, 0.0f, 1.0f)
#define PX_OLIVE px::Color(0.5f, 0.5f, 0.0f, 1.0f)
#define PX_NAVY px::Color(0.0f, 0.0f, 0.5f, 1.0f)
#define PX_TEAL px::Color(0.0f, 0.5f, 0.5f, 1.0f)
#define PX_LIME px::Color(0.75f, 1.0f, 0.0f, 1.0f)
#define PX_GOLD px::Color(1.0f, 0.84f, 0.0f, 1.0f)
#define PX_SILVER px::Color(0.75f, 0.75f, 0.75f, 1.0f)
#define PX_BEIGE px::Color(0.96f, 0.96f, 0.86f, 1.0f)
#define PX_IVORY px::Color(1.0f, 1.0f, 0.94f, 1.0f)
#define PX_CORAL px::Color(1.0f, 0.5f, 0.31f, 1.0f)
#define PX_SALMON px::Color(0.98f, 0.5f, 0.45f, 1.0f)
#define PX_TURQUOISE px::Color(0.25f, 0.88f, 0.82f, 1.0f)
#define PX_AQUAMARINE px::Color(0.5f, 1.0f, 0.83f, 1.0f)
#define PX_VIOLET px::Color(0.93f, 0.51f, 0.93f, 1.0f)
#define PX_INDIGO px::Color(0.29f, 0.0f, 0.51f, 1.0f)
#define PX_KHAKI px::Color(0.76f, 0.69f, 0.57f, 1.0f)
#define PX_CHOCOLATE px::Color(0.82f, 0.41f, 0.12f, 1.0f)
#define PX_CRIMSON px::Color(0.86f, 0.08f, 0.24f, 1.0f)
#define PX_LAVENDER px::Color(0.9f, 0.9f, 0.98f, 1.0f)
#define PX_SKY_BLUE px::Color(0.53f, 0.81f, 0.98f, 1.0f)

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