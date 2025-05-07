#include "pixl/core/math/MathFunctions.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace px;

float px::Math::Lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

px::Vec2 px::Math::Lerp(const Vec2& a, const Vec2& b, float f)
{
    return Vec2(Lerp(a.x, b.x, f), Lerp(a.y, b.y, f));
}

bool px::Math::LineRectIntersection(float rP1, float rP2, float rS1, float rS2, float lp1, float lp1_1, float lp2, float lp2_1)
{
    return ((lp1 >= rP1 && lp1 <= (rP1 + rS1)) && (lp1_1 >= rP2 && lp1_1 <= (rP2 + rS2))) || ((lp2 >= rP1 && lp2 <= (rP1 + rS1)) && (lp2_1 >= rP2 && lp2_1 <= (rP2 + rS2)));
}

bool px::Math::RectRectIntersection(const Vec2& rect1Pos, const Vec2& rect1Size, const Vec2& rect2Pos, const Vec2& rect2Size)
{
    float tw = rect1Size.x;
    float th = rect1Size.y;
    float rw = rect2Size.x;
    float rh = rect2Size.y;
    if (rw <= 0 || rh <= 0 || tw <= 0 || th <= 0) {
        return false;
    }
    float tx = rect1Pos.x;
    float ty = rect1Pos.y;
    float rx = rect2Pos.x;
    float ry = rect2Pos.y;
    rw += rx;
    rh += ry;
    tw += tx;
    th += ty;
    return ((rw < rx || rw > tx) &&
        (rh < ry || rh > ty) &&
        (tw < tx || tw > rx) &&
        (th < ty || th > ry));
}

float px::Math::ToRadians(float angle)
{
    return angle * (M_PI / 180.0f);
}
