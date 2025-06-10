#include "pixl/core/graphics/SimpleRenderer.h"
#include "pixl/core/pixl.h"

#include <glad/glad.h>
#include <iostream>

static const char* __pixl_simple_shader_vert = R"(
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;

out vec2 px_uv;

uniform mat4 projection_matrix;
uniform mat4 model_matrix;

void main()
{
	gl_Position = projection_matrix * model_matrix * vec4(pos, 1.0);
	px_uv = texCoord;
}
)";

static const char* __pixl_simple_shader_frag = R"(
void main()
{
    gl_FragColor = px_color;
}
)";

using namespace px;

px::SimpleRenderer::SimpleRenderer(const SimpleDrawCallback& callback) : m_Callback(callback), m_Drawing(false)
{
}

px::SimpleRenderer::~SimpleRenderer()
{
    if (m_Shader) delete m_Shader;
}

void px::SimpleRenderer::Construct()
{
    m_Shader = new Shader(__pixl_simple_shader_vert, __pixl_simple_shader_frag, true);
    m_Shader->Use();
    m_Shader->SetMatrix4("projection_matrix", Mat4::Ortho(0.0f, m_Wnd->GetFixedSize().x, m_Wnd->GetFixedSize().y, 0.0f));
}

PipelineData px::SimpleRenderer::Downstream(const PipelineData& data)
{
    if (!data.prevBuf) return data;
    data.prevBuf->Bind();

    m_Shader->Use();

    m_Ctx = data.ctx;
    m_Drawing = true;
    if (m_Callback)
    {
        m_Callback(this);
    }
    m_Drawing = false;

    data.prevBuf->Unbind();
    return data;
}

void px::SimpleRenderer::DrawRect(const Vec2& pos, const Vec2& size, const Color& color)
{
    if (!m_Drawing) return;

    Mat4 mat;
    mat.Translate(pos);
    mat.Scale(size);

    m_Shader->SetMatrix4("model_matrix", mat);
    m_Shader->SetColor("px_color", color);

    m_Ctx->DrawQuadOutline();
}
