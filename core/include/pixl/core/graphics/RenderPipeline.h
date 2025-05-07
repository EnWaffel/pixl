#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/math/Vec2.h"
#include "PipelineElement.h"

namespace px
{
    class RenderPipelineImpl;
    class Window;
    typedef Window* WINDOW;

    class RenderPipeline
    {
    public:
        PX_API RenderPipeline(WINDOW window);
        PX_API ~RenderPipeline();

        PX_API void AddElement(CREFSTR name, PIPEELEMENT element);
        PX_API void AddElementBefore(CREFSTR other, CREFSTR name, PIPEELEMENT element);
        PX_API void AddElementAfter(CREFSTR other, CREFSTR name, PIPEELEMENT element);
        PX_API void RemoveElement(CREFSTR name);
        PX_API PIPEELEMENT GetElement(CREFSTR name);

        PX_API void Flush();
    private:
        RenderPipelineImpl* m_Impl;
    };

    typedef RenderPipeline* RENDERPIPELINE;
}