#include "pixl/core/graphics/CameraRenderer.h"
#include "pixl/core/pixl.h"

#include <glad/glad.h>
#include <iostream>

static const char* __pixl_sprite_shader_frag = R"(
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

    vec4 c = texture(px_texture, px_uv * px_uv_size + px_uv_coord);
    gl_FragColor = c * px_color;
}
)";

static const char* __pixl_text_shader_vert = R"(
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

static const char* __pixl_text_shader_frag = R"(
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
    class __pixl_cam_rend_evt
    {
    public:
        static bool __pixl_cam_rend_on_window_event(const EventData& data, void* userData)
        {
            if (!userData) return true;
            const WindowChangedData& _data = (const WindowChangedData&)data;
            CameraRenderer* obj = (CameraRenderer*)userData;

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
    if (m_SpriteShader) delete m_SpriteShader;
    if (m_TextShader) delete m_TextShader;
    if (m_Framebuf) delete m_Framebuf;
}

void px::CameraRenderer::Construct()
{
    m_SpriteShader = new Shader(__pixl_sprite_shader_frag);
    m_SpriteShader->Use();
    m_SpriteShader->SetMatrix4("projection_matrix", Mat4::Ortho(0.0f, m_Wnd->GetFixedSize().x, m_Wnd->GetFixedSize().y, 0.0f));

    m_TextShader = new Shader(__pixl_text_shader_vert, __pixl_text_shader_frag, true);
    m_TextShader->Use();
    m_TextShader->SetMatrix4("projection_matrix", Mat4::Ortho(0.0f, m_Wnd->GetFixedSize().x, m_Wnd->GetFixedSize().y, 0.0f));

    m_Framebuf = new Framebuffer(m_Wnd->GetFixedSize(), true);

    m_Wnd->GetEventManager()->AddListener("__pixl_cam_rend#" + std::to_string((uint64_t)this), PX_EVENT(WINDOW_CHANGED), __pixl_cam_rend_evt::__pixl_cam_rend_on_window_event, ListenerPriority::HIGH, this);
}

PipelineData px::CameraRenderer::Downstream(const PipelineData& data)
{
    if (!data.camera) return data;

    m_Framebuf->Bind();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    data.camera->Draw(data.ctx, m_SpriteShader, m_TextShader);

    m_Framebuf->Unbind();

    PipelineData d = data;
    d.prevBuf = m_Framebuf;
    return d;
}
