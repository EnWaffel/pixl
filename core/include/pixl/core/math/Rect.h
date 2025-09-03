#pragma once

#include "pixl/utility/Defs.h"

namespace px
{
#pragma pack(push, 1)
    class Rect
    {
    public:
        float x;
        float y;
        float w;
        float h;
    public:
        PX_API Rect();
        PX_API Rect(float x, float y, float w, float h);
    };
#pragma pack(pop)
}