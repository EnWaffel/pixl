#pragma once

#include "pixl/utility/Defs.h"
#include "Vec2.h"

namespace px
{
    class Math
    {
    public:
        PX_API static float Lerp(float a, float b, float f);
        PX_API static Vec2 Lerp(const Vec2& a, const Vec2& b, float f);
        PX_API static bool LineRectIntersection(float rP1, float rP2, float rS1, float rS2, float lp1, float lp1_1, float lp2, float lp2_1);
        PX_API static bool RectRectIntersection(const Vec2& rect1Pos, const Vec2& rect1Size, const Vec2& rect2Pos, const Vec2& rect2Size);
        PX_API static float ToRadians(float angle);
    };
}