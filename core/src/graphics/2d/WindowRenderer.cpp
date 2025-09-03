#include "pixl/core/graphics/2d/WindowRenderer.h"
#include "pixl/core/pixl.h"

#include <glad/glad.h>

using namespace px;

namespace px
{
    class __pixl_wnd_rend_evt
    {
    public:
        static bool __pixl_wnd_rend_on_window_event(const EventData& data, void* userData)
        {
            if (!userData) return true;
            const WindowChangedData& _data = (const WindowChangedData&)data;
            WindowRenderer* obj = (WindowRenderer*)userData;

            obj->m_ViewportPos = _data.viewportPos;
            obj->m_ViewportSize = _data.viewportSize;

            return true;
        }
    };
};

px::WindowRenderer::WindowRenderer()
{
}

px::WindowRenderer::~WindowRenderer()
{
    m_Wnd->GetEventManager()->RemoveListener("__pixl_wnd_rend#" + std::to_string((uint64_t)this));
    if (m_WindowShader) delete m_WindowShader;
}

void px::WindowRenderer::Construct()
{
    m_WindowShader = ShaderCodeBuilder::NewDefault().WindowVertex().WindowFragment().Compile();
    m_WindowShader->Use();

    m_Wnd->GetEventManager()->AddListener("__pixl_wnd_rend#" + std::to_string((uint64_t)this), PX_EVENT(WINDOW_CHANGED), __pixl_wnd_rend_evt::__pixl_wnd_rend_on_window_event, ListenerPriority::HIGH, this);
}

PipelineData px::WindowRenderer::Downstream(const PipelineData& data)
{
    if (!data.prevBuf) return data;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(m_ViewportPos.x, m_ViewportPos.y, m_ViewportSize.x, m_ViewportSize.y);

    data.prevBuf->BindForDrawing();

    m_WindowShader->Use();
    data.ctx->DrawScreenQuad();

    glBindTexture(GL_TEXTURE_2D, 0);
    return data;
}
