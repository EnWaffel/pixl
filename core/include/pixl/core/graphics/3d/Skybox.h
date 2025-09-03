#pragma once

#include "pixl/core/graphics/Drawable.h"
#include "pixl/core/math/Vec3.h"
#include "pixl/core/asset/CubeMap.h"

namespace px
{
    class PX_API Skybox : public Drawable
    {
    public:
        CUBEMAP cubeMap = nullptr;
        Vec3 rotation;
    public:
        PX_API Skybox(CUBEMAP cubeMap);

        PX_API void Draw(const DrawData& data);
        PX_API void Update(float delta);
    };
}