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
    gl_FragColor = c;
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
    if (m_Framebuf) delete m_Framebuf;
}

void px::CameraRenderer::Construct()
{
    m_SpriteShader = new Shader(__pixl_sprite_shader_frag);
    m_SpriteShader->Use();
    m_SpriteShader->SetMatrix4("projection_matrix", Mat4::Ortho(0.0f, m_Wnd->GetFixedSize().x, m_Wnd->GetFixedSize().y, 0.0f));

    m_Framebuf = new Framebuffer(m_Wnd->GetFixedSize(), true);

    m_Wnd->GetEventManager()->AddListener("__pixl_cam_rend#" + std::to_string((uint64_t)this), PX_EVENT(WINDOW_CHANGED), __pixl_cam_rend_evt::__pixl_cam_rend_on_window_event, ListenerPriority::HIGH, this);
}

PipelineData px::CameraRenderer::Downstream(const PipelineData& data)
{
    m_Framebuf->Bind();

    glViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    data.camera->Draw(data.ctx, m_SpriteShader);

    m_Framebuf->Unbind();

    PipelineData d = data;
    d.prevBuf = m_Framebuf;
    return d;
}
