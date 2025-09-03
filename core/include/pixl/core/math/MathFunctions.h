#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/core/math/Vec2.h"
#include "pixl/core/math/Vec3.h"
#include "pixl/core/math/Rect.h"

namespace px
{
    class Math
    {
    public:
        PX_API static float Lerp(float a, float b, float f);
        PX_API static Vec2 Lerp(const Vec2& a, const Vec2& b, float f);
        PX_API static Vec3 Lerp(const Vec3& a, const Vec3& b, float f);
        PX_API static bool LineRectIntersection(float rP1, float rP2, float rS1, float rS2, float lp1, float lp1_1, float lp2, float lp2_1);
        PX_API static bool RectRectIntersection(const Vec2& rect1Pos, const Vec2& rect1Size, const Vec2& rect2Pos, const Vec2& rect2Size);
        PX_API static bool RectRectIntersection(const Rect& rect1, const Rect& rect2);
        PX_API static bool PointRectIntersection(const Vec2& point, const Rect& rect);
        PX_API static float ToRadians(float angle);
        
        template<typename T>
        static inline T Max(T first, T second)
        {
            return first > second ? first : second;
        }

        template<typename T>
        static inline T Min(T first, T second)
        {
            return first < second ? first : second;
        }
    };
}