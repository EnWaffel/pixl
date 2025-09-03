#include "pixl/core/graphics/2d/SimpleRenderer.h"
#include "pixl/core/pixl.h"

#include <glad/glad.h>

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
    m_Shader = ShaderCodeBuilder::NewDefault().BasicVertex().BasicFragment().Compile();
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
    m_Shader->SetBool("px_only_color", true);

    m_Ctx->DrawQuadOutline();
}
