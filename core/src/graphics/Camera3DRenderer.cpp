#include "pixl/core/graphics/Camera3DRenderer.h"
#include "pixl/core/pixl.h"

#include <glad/glad.h>

static const char* __pixl_obj_shader_vert = R"(
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;

out vec2 px_uv;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

void main()
{
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(pos, 1.0);
	px_uv = texCoord;
}
)";

static const char* __pixl_obj_shader_frag = R"(
void main()
{
	vec2 uv = px_uv;
	if (px_flip_x)
	{
		uv.x = 1.0 - uv.x;
	}
	if (px_flip_y)
	{
		uv.y = 1.0 - uv.y;
	}

    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(px_texture, uv).r);
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0) * sampled;
    gl_FragColor *= px_color;
}
)";

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
    if (m_Framebuf) delete m_Framebuf;
}

void px::Camera3DRenderer::Construct()
{
    m_ObjectShader = new Shader(__pixl_obj_shader_vert, __pixl_obj_shader_frag, true);
    m_ObjectShader->Use();
    m_ObjectShader->SetMatrix4("projection_matrix", Mat4::Perspective(45.0f, m_Wnd->GetFixedSize().x / m_Wnd->GetFixedSize().y, 0.0f, 10000.0f));

    m_Framebuf = new Framebuffer(m_Wnd->GetFixedSize(), true);

    m_Wnd->GetEventManager()->AddListener("__pixl_cam3d_rend#" + std::to_string((uint64_t)this), PX_EVENT(WINDOW_CHANGED), __pixl_cam3d_rend_evt::__pixl_cam3d_rend_on_window_event, ListenerPriority::HIGH, this);
}

PipelineData px::Camera3DRenderer::Downstream(const PipelineData& data)
{
    //m_Framebuf->Bind();

    glViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    

    //m_Framebuf->Unbind();

    //PipelineData d = data;
    //d.prevBuf = m_Framebuf;
    return data;
}
