#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/graphics/PipelineElement.h"
#include "pixl/core/graphics/Shader.h"
#include "pixl/core/graphics/Framebuffer.h"
#include "pixl/core/math/Vec2i.h"

namespace px
{
    class __pixl_prt_rend_evt;

    class ParticleRenderer : public PipelineElement
    {
    public:
        PX_API ParticleRenderer();
        PX_API ~ParticleRenderer();

        PX_API void Construct() override;
        PX_API PipelineData Downstream(const PipelineData& data) override;
    private:
        FRAMEBUF m_Framebuf;
        Vec2i m_ViewportSize;

        friend class __pixl_prt_rend_evt;
    };
}