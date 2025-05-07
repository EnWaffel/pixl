#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/math/Vec2.h"
#include "pixl/core/math/Mat4.h"
#include "DrawingContext.h"
#include "Shader.h"

namespace px
{
    struct PX_API DrawData
    {
        DRAWINGCTX ctx;
        Mat4 viewMatrix;
        void* data;
        SHADER shd;
    };

    class Drawable
    {
    public:
        Vec2 pos;
        Vec2 size;
        Vec2 scale = Vec2(1.0f, 1.0f);
        bool visible = true;
        bool flipX = false;
        bool flipY = false;
        int order = -1; // -1 == ignore order (always in the back)
    public:
        PX_API virtual void Draw(const DrawData& data) = 0;
        PX_API virtual void Update(float delta) = 0;
    };

    typedef Drawable* DRAWABLE;
}