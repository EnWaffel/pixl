#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "PipelineElement.h"
#include "pixl/core/graphics/Shader.h"
#include "pixl/core/graphics/Framebuffer.h"
#include "pixl/core/math/Vec2i.h"

namespace px
{
    class __pixl_cam_rend_evt;

    class CameraRenderer : public PipelineElement
    {
    public:
        PX_API CameraRenderer();
        PX_API ~CameraRenderer();

        PX_API void Construct() override;
        PX_API PipelineData Downstream(const PipelineData& data) override;
    private:
        SHADER m_SpriteShader;
        SHADER m_TextShader;
        FRAMEBUF m_Framebuf;
        Vec2i m_ViewportSize;

        friend class __pixl_cam_rend_evt;
    };
}