#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/math/Vec2.h"
#include "pixl/core/graphics/2d/Camera.h"
#include "pixl/core/graphics/3d/Camera3D.h"
#include "pixl/core/graphics/DrawingContext.h"
#include "pixl/core/graphics/Framebuffer.h"

namespace px
{
    struct PX_API PipelineData
    {
        DRAWINGCTX ctx = nullptr;
        CAMERA camera = nullptr;
        CAMERA3D camera3d = nullptr;
        bool final = false;
        FRAMEBUF prevBuf = nullptr;
    };

    class PipelineElement
    {
    public:
        PipelineElement() = default;
        virtual ~PipelineElement() = default;

        PX_API virtual void Construct() = 0;
        PX_API virtual PipelineData Downstream(const PipelineData& data) = 0;
    protected:
        WINDOW m_Wnd;

        friend class RenderPipelineImpl;
    };

    typedef PipelineElement* PIPEELEMENT;
}