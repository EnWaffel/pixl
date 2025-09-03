#include "pixl/core/graphics/ParticleRenderer.h"
#include "pixl/core/pixl.h"

#include <glad/glad.h>

using namespace px;

namespace px
{
    class __pixl_prt_rend_evt
    {
    public:
        static bool __pixl_prt_rend_on_window_event(const EventData& data, void* userData)
        {
            if (!userData) return true;
            const WindowChangedData& _data = (const WindowChangedData&)data;
            ParticleRenderer* obj = (ParticleRenderer*)userData;

            obj->m_ViewportSize = _data.viewportSize;

            delete obj->m_Framebuf;
            obj->m_Framebuf = new Framebuffer(_data.viewportSize, true);

            return true;
        }
    };
};

px::CameraRenderer::CameraRenderer()
{
}

px::CameraRenderer::~CameraRenderer()
{
    m_Wnd->GetEventManager()->RemoveListener("__pixl_prt_rend#" + std::to_string((uint64_t)this));
    if (m_Framebuf) delete m_Framebuf;
}

void px::CameraRenderer::Construct()
{
    m_Framebuf = new Framebuffer(m_Wnd->GetViewportSize(), true);

    m_Wnd->GetEventManager()->AddListener("__pixl_prt_rend#" + std::to_string((uint64_t)this), PX_EVENT(WINDOW_CHANGED), __pixl_prt_rend_evt::__pixl_prt_rend_on_window_event, ListenerPriority::HIGH, this);
}

PipelineData px::CameraRenderer::Downstream(const PipelineData& data)
{
    if (!data.camera) return data;

    m_Framebuf->Bind();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    

    m_Framebuf->Unbind();

    PipelineData d = data;
    d.prevBuf = m_Framebuf;
    return d;
}
