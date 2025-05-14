#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/math/Vec2.h"
#include "pixl/core/math/Mat4.h"
#include "pixl/core/graphics/DrawingContext.h"
#include "pixl/core/graphics/Shader.h"

namespace px
{
    struct PX_API DrawData
    {
        DRAWINGCTX ctx;
        Mat4 viewMatrix;
        void* data;
        SHADER shd; // Sprite Shader
        SHADER shd1; // Text Shader
        SHADER shd2;
        SHADER shd3;
    };

    class Drawable
    {
    public:
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