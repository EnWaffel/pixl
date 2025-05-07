#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "PipelineElement.h"
#include "pixl/core/graphics/Shader.h"
#include "pixl/core/graphics/Framebuffer.h"
#include "pixl/core/math/Vec2i.h"

namespace px
{
    class __pixl_wnd_rend_evt;

    class WindowRenderer : public PipelineElement
    {
    public:
        PX_API WindowRenderer();
        PX_API ~WindowRenderer();

        PX_API void Construct() override;
        PX_API PipelineData Downstream(const PipelineData& data) override;
    private:
        SHADER m_WindowShader;
        Vec2i m_ViewportPos;
        Vec2i m_ViewportSize;

        friend class __pixl_wnd_rend_evt;
    };
}