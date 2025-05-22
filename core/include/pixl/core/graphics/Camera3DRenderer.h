#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/graphics/PipelineElement.h"
#include "pixl/core/graphics/Shader.h"
#include "pixl/core/graphics/Framebuffer.h"
#include "pixl/core/math/Vec2i.h"

namespace px
{
    class __pixl_cam3d_rend_evt;

    class Camera3DRenderer : public PipelineElement
    {
    public:
        PX_API Camera3DRenderer();
        PX_API ~Camera3DRenderer();

        PX_API void Construct() override;
        PX_API PipelineData Downstream(const PipelineData& data) override;
    private:
        SHADER m_ObjectShader;
        FRAMEBUF m_Framebuf;
        Vec2i m_ViewportSize;

        friend class __pixl_cam3d_rend_evt;
    };
}