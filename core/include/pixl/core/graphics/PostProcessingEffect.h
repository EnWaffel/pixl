#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/graphics/PipelineElement.h"
#include "pixl/core/graphics/Shader.h"
#include "pixl/core/graphics/Framebuffer.h"
#include "pixl/core/math/Vec2i.h"
#include "pixl/core/graphics/2d/Camera.h"

#include <vector>

namespace px
{
    class __pixl_ppe_rend_evt;

    class PostProcessingEffect : public PipelineElement
    {
    public:
        PX_API PostProcessingEffect(CREFSTR fragmentCode);
        PX_API ~PostProcessingEffect();

        PX_API void Construct() override;
        PX_API PipelineData Downstream(const PipelineData& data) override;

        PX_API void HideFrom(CAMERA camera);
        PX_API void ShowTo(CAMERA camera);

        PX_API SHADER GetShader();
    private:
        SHADER m_Shader;
        FRAMEBUF m_Framebuf;
        Vec2i m_ViewportSize;
        std::string m_FragmentCode;
        std::vector<CAMERA> m_HiddenCameras;

        friend class __pixl_ppe_rend_evt;
    };

    typedef PostProcessingEffect* PPEffect;
}