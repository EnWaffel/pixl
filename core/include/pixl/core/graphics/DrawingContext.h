#pragma once

#include "pixl/utility/Defs.h"

namespace px
{
    class DrawingContext
    {
    public:
        PX_INTERNAL DrawingContext();
        PX_INTERNAL ~DrawingContext();

        PX_API void DrawQuad();
        PX_API void DrawQuadOutline();
        PX_API void DrawScreenQuad();
    private:
        unsigned int m_Data[2];
        unsigned int m_Data1[2];
    };

    typedef DrawingContext* DRAWINGCTX;
}