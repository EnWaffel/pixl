#include "pixl/core/graphics/WindowRenderer.h"
#include "pixl/core/pixl.h"

#include <glad/glad.h>
#include <iostream>

static const char* __pixl_window_shader_vert = R"(
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;

out vec2 px_uv;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

void main()
{
    gl_Position = vec4(pos, 1.0);
    px_uv = texCoord;
}
)";

static const char* __pixl_window_shader_frag = R"(
void main()
{
    gl_FragColor = texture(px_texture, px_uv);
}
)";

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
    if (m_WindowShader) delete m_WindowShader;
}

void px::WindowRenderer::Construct()
{
    m_WindowShader = new Shader(__pixl_window_shader_vert, __pixl_window_shader_frag, true);
    m_WindowShader->Use();

    m_Wnd->GetEventManager()->AddListener("__pixl_wnd_rend#" + std::to_string((uint64_t)this), PX_EVENT(WINDOW_CHANGED), __pixl_wnd_rend_evt::__pixl_wnd_rend_on_window_event, ListenerPriority::HIGH, this);
}

PipelineData px::WindowRenderer::Downstream(const PipelineData& data)
{
    if (!data.prevBuf) return data;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(m_ViewportPos.x, m_ViewportPos.y, m_ViewportSize.x, m_ViewportSize.y);

    data.prevBuf->BindForDrawing();

    m_WindowShader->Use();
    data.ctx->DrawScreenQuad();

    glBindTexture(GL_TEXTURE_2D, 0);
    return data;
}
