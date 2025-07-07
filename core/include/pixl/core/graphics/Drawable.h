#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/math/Vec2.h"
#include "pixl/core/math/Mat4.h"
#include "pixl/core/graphics/DrawingContext.h"
#include "pixl/core/graphics/Shader.h"

namespace px
{
    class Window;

    struct PX_API DrawData
    {
        Vec2 offset;
        Vec2 scale;
        DRAWINGCTX ctx;
        Mat4 projectionMatrix;
        Mat4 viewMatrix;
        void* data;
        SHADER shd; // Sprite Shader
        SHADER shd1; // Text Shader
        SHADER shd2;
        SHADER shd3;
        Window* wnd;
    };

    class Drawable
    {
    public:
        bool visible = true;
        int order = -1; // -1 == ignore order (always in the back)
    public:
        virtual ~Drawable() = default;

        PX_API virtual void Draw(const DrawData& data) = 0;
        PX_API virtual void Update(float delta) = 0;
    };

    typedef Drawable* DRAWABLE;
}