#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/core/math/Vec3.h"
#include "pixl/utility/Color.h"

namespace px
{
    struct Light
    {
        Vec3 pos;
        Color color;
        float ambientStrength;
        float diffuseStrength;
        float specularStrength;
    };
}