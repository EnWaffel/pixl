#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/math/Vec2.h"
#include "pixl/core/math/Mat4.h"
#include "pixl/core/graphics/DrawingContext.h"
#include "pixl/core/graphics/Shader.h"

#include <vector>

#define PX_SHD_IDX_SPRITE 0
#define PX_SHD_IDX_TEXT 1
#define PX_SHD_IDX_OBJECT 0
#define PX_SHD_IDX_SKYBOX 1

namespace px
{
    class Window;

    struct PX_API DrawData
    {
        Window* wnd;
        DRAWINGCTX ctx;
        // Camera values
        Vec2 offset;
        Vec2 scale;
        // Matrices
        Mat4 projectionMatrix;
        Mat4 viewMatrix;
        /**
         * Indices:
         * 
         * 2D:
         *   0 = Sprite Shader
         *   1 = Text Shader
         * 3D:
         *   0 = Object Shader
         *   1 = Skybox Shader
         */
        SHADER shaders[8];
        // Userdata
        void* data;
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