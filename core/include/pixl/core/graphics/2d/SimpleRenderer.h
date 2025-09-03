#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/graphics/PipelineElement.h"
#include "pixl/core/graphics/Shader.h"
#include "pixl/core/graphics/Framebuffer.h"
#include "pixl/core/math/Vec2i.h"

#include <functional>

namespace px
{
    class SimpleRenderer;

    typedef std::function<void(SimpleRenderer*)> SimpleDrawCallback;

    class SimpleRenderer : public PipelineElement
    {
    public:
        PX_API SimpleRenderer(const SimpleDrawCallback& callback);
        PX_API ~SimpleRenderer();

        PX_API void Construct() override;
        PX_API PipelineData Downstream(const PipelineData& data) override;

        PX_API void DrawRect(const Vec2& pos, const Vec2& size, const Color& color);
    private:
        SHADER m_Shader;
        SimpleDrawCallback m_Callback;
        bool m_Drawing;
        DRAWINGCTX m_Ctx;
    };
}