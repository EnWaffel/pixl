#include "pixl/core/graphics/3d/Camera3DRenderer.h"
#include "pixl/core/pixl.h"
#include "pixl/core/Shaders.h"

#include <glad/glad.h>

using namespace px;

namespace px
{
    class __pixl_cam3d_rend_evt
    {
    public:
        static bool __pixl_cam3d_rend_on_window_event(const EventData& data, void* userData)
        {
            if (!userData) return true;
            const WindowChangedData& _data = (const WindowChangedData&)data;
            Camera3DRenderer* obj = (Camera3DRenderer*)userData;

            obj->m_ViewportSize = _data.viewportSize;

            delete obj->m_Framebuf;
            obj->m_Framebuf = new Framebuffer(_data.viewportSize, true);

            return true;
        }
    };
};

px::Camera3DRenderer::Camera3DRenderer()
{
}

px::Camera3DRenderer::~Camera3DRenderer()
{
    if (m_ObjectShader) delete m_ObjectShader;
    if (m_SkyboxShader) delete m_SkyboxShader;
    if (m_Framebuf) delete m_Framebuf;
}

void px::Camera3DRenderer::Construct()
{
    glEnable(GL_DEPTH_TEST);

    m_ObjectShader = ShaderCodeBuilder::NewDefault().Vertex(PX_SHADER_VERTEX_3D).Fragment(PX_SHADER_FRAGMENT_3D, false).Compile();
    m_SkyboxShader = ShaderCodeBuilder::NewDefault().Vertex(PX_SHADER_VERTEX_SKYBOX).Fragment(PX_SHADER_FRAGMENT_SKYBOX, false).Compile();

    m_Framebuf = new Framebuffer(m_Wnd->GetFixedSize(), true);

    m_Wnd->GetEventManager()->AddListener("__pixl_cam3d_rend#" + std::to_string((uint64_t)this), PX_EVENT(WINDOW_CHANGED), __pixl_cam3d_rend_evt::__pixl_cam3d_rend_on_window_event, ListenerPriority::HIGH, this);
}

PipelineData px::Camera3DRenderer::Downstream(const PipelineData& data)
{
    if (!data.camera3d) return data;

    glEnable(GL_DEPTH_TEST);
    glViewport(m_Wnd->GetViewportPos().x, m_Wnd->GetViewportPos().y, m_ViewportSize.x, m_ViewportSize.y);
    data.camera3d->Draw(data.ctx, m_ObjectShader, m_SkyboxShader);

    return data;
}
