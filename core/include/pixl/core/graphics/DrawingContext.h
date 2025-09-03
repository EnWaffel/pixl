#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/core/math/Vec2i.h"

namespace px
{
    class Window;

    class DrawingContext
    {
    public:
        PX_INTERNAL DrawingContext();
        PX_INTERNAL ~DrawingContext();

        PX_API void DrawQuad();
        PX_API void DrawQuadOutline();
        PX_API void DrawScreenQuad();
        PX_API void DrawSkybox();

        PX_API void Scissor(int x, int y, int width, int height, Window* wnd = nullptr);
        PX_API void Scissor(const Vec2i& pos, const Vec2i& size, Window* wnd = nullptr);
        PX_API void NoScissor();
    private:
        unsigned int m_Data[3]; // VAOs
        unsigned int m_Data1[3]; // VBOs
    };

    typedef DrawingContext* DRAWINGCTX;
}