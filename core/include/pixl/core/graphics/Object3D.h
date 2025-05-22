#pragma once

#include "pixl/core/graphics/Drawable.h"
#include "pixl/core/math/Vec3.h"
#include "pixl/core/asset/Model.h"

namespace px
{
    class Object3D : public Drawable
    {
    public:
        Vec3 pos;
        Vec3 scale;
        Vec3 rotation;
        MODEL model;
    public:
        PX_API Object3D();

        PX_API void Draw(const DrawData& data);
        PX_API void Update(float delta);
    };
}