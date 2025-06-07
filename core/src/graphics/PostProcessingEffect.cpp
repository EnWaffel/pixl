#include "pixl/core/graphics/PostProcessingEffect.h"
#include "pixl/core/pixl.h"

#include <glad/glad.h>
#include <algorithm>

static const char* __pixl_ppe_shader_vert = R"(
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;

out vec2 px_uv;

void main()
{
    gl_Position = vec4(pos, 1.0);
    px_uv = texCoord;
}
)";

using namespace px;

namespace px
{
    class __pixl_ppe_rend_evt
    {
    public:
        static bool __pixl_ppe_rend_on_window_event(const EventData& data, void* userData)
        {
            if (!userData) return true;
            const WindowChangedData& _data = (const WindowChangedData&)data;
            PPEffect obj = (PPEffect)userData;

            obj->m_ViewportSize = _data.viewportSize;

            delete obj->m_Framebuf;
            obj->m_Framebuf = new Framebuffer(_data.viewportSize, true);

            return true;
        }
    };
};

px::PostProcessingEffect::PostProcessingEffect(CREFSTR fragmentCode) : m_FragmentCode(fragmentCode)
{
}

px::PostProcessingEffect::~PostProcessingEffect()
{
    m_Wnd->GetEventManager()->RemoveListener("__pixl_ppe_rend#" + std::to_string((uint64_t)this));
    if (m_Shader) delete m_Shader;
    if (m_Framebuf) delete m_Framebuf;
}

void px::PostProcessingEffect::Construct()
{
    m_Shader = new Shader(__pixl_ppe_shader_vert, m_FragmentCode, true);
    m_Shader->Use();
    m_Shader->SetVec2("px_resolution", m_Wnd->GetFixedSize());

    m_Framebuf = new Framebuffer(m_Wnd->GetViewportSize(), true);

    m_Wnd->GetEventManager()->AddListener("__pixl_ppe_rend#" + std::to_string((uint64_t)this), PX_EVENT(WINDOW_CHANGED), __pixl_ppe_rend_evt::__pixl_ppe_rend_on_window_event, ListenerPriority::HIGH, this);
}

PipelineData px::PostProcessingEffect::Downstream(const PipelineData& data)
{
    if (!data.prevBuf) return data;
    for (CAMERA camera : m_HiddenCameras)
    {
        if (data.camera == camera) return data;
    }

    m_Framebuf->Bind();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_Shader->Use();

    data.prevBuf->BindForDrawing();
    data.ctx->DrawScreenQuad();

    m_Framebuf->Unbind();

    PipelineData d = data;
    d.prevBuf = m_Framebuf;
    return d;
}

void px::PostProcessingEffect::HideFrom(CAMERA camera)
{
    m_HiddenCameras.push_back(camera);
}

void px::PostProcessingEffect::ShowTo(CAMERA camera)
{
    auto it = std::find(m_HiddenCameras.begin(), m_HiddenCameras.end(), camera);
    if (it == m_HiddenCameras.end()) return;
    m_HiddenCameras.erase(it);
}

SHADER px::PostProcessingEffect::GetShader()
{
    return m_Shader;
}
