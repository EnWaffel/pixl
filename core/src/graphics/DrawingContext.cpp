#include "pixl/core/graphics/DrawingContext.h"
#include "pixl/core/window/Window.h"

#include <glad/glad.h>

using namespace px;

extern WINDOW __pixl_rootwnd;

px::DrawingContext::DrawingContext()
{
    PX_DEBUG_LOG("DrawingContext::DrawingContext()", "New DrawingContext");
    
    glGenVertexArrays(3, m_Data);
    glGenBuffers(3, m_Data1);

    float quadVertices[] = {
        // first triangle
        1.0f,  1.0f, 0.0f, 1.0, 1.0,  // top right
        1.0f,  0.0f, 0.0f, 1.0, 0.0,  // bottom right
        0.0f,  1.0f, 0.0f, 0.0, 1.0, // top left
        // second triangle
        1.0f,  0.0f, 0.0f, 1.0, 0.0, // bottom right
        0.0f,  0.0f, 0.0f, 0.0, 0.0, // bottom left
        0.0f,  1.0f, 0.0f, 0.0, 1.0 // top left
    };

    glBindVertexArray(m_Data[0]);

    glBindBuffer(GL_ARRAY_BUFFER, m_Data1[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (sizeof(quadVertices) / 5) * 3 + sizeof(float) * (sizeof(quadVertices) / 5) * 2, quadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    PX_DEBUG_LOG("DrawingContext::DrawingContext()", "Quad Buffer done");

    float screenQuadVertices[] = {
        // first triangle
        1.0f,  1.0f, 0.0f, 1.0, 1.0,  // top right
        1.0f,  -1.0f, 0.0f, 1.0, 0.0,  // bottom right
        -1.0f,  1.0f, 0.0f, 0.0, 1.0, // top left
        // second triangle
        1.0f,  -1.0f, 0.0f, 1.0, 0.0, // bottom right
        -1.0f,  -1.0f, 0.0f, 0.0, 0.0, // bottom left
        -1.0f,  1.0f, 0.0f, 0.0, 1.0 // top left
    };

    glBindVertexArray(m_Data[1]);

    glBindBuffer(GL_ARRAY_BUFFER, m_Data1[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (sizeof(screenQuadVertices) / 5) * 3 + sizeof(float) * (sizeof(screenQuadVertices) / 5) * 2, screenQuadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    PX_DEBUG_LOG("DrawingContext::DrawingContext()", "Screen Quad Buffer done");

    float skyboxVertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    glBindVertexArray(m_Data[2]);

    glBindBuffer(GL_ARRAY_BUFFER, m_Data1[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (sizeof(skyboxVertices) / 5) * 3 + sizeof(float) * (sizeof(skyboxVertices) / 5) * 2, skyboxVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    PX_DEBUG_LOG("DrawingContext::DrawingContext()", "Skybox Buffer done");
}

px::DrawingContext::~DrawingContext()
{
    glDeleteVertexArrays(2, m_Data);
    glDeleteBuffers(2, m_Data1);
}

void px::DrawingContext::DrawQuad()
{
    glBindVertexArray(m_Data[0]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void px::DrawingContext::DrawQuadOutline()
{
    glBindVertexArray(m_Data[0]);
	glDrawArrays(GL_LINE_LOOP, 0, 6);
	glBindVertexArray(0);
}

void px::DrawingContext::DrawScreenQuad()
{
	glBindVertexArray(m_Data[1]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void px::DrawingContext::DrawSkybox()
{
    glBindVertexArray(m_Data[2]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void px::DrawingContext::Scissor(int x, int y, int width, int height, WINDOW wnd)
{
    WINDOW _wnd = wnd;
    if (!_wnd) _wnd = __pixl_rootwnd;

    Vec2i viewportPos = _wnd->GetViewportPos();
    Vec2i viewportSize = _wnd->GetViewportSize();

    float scaleX = (float)viewportSize.x / _wnd->GetFixedSize().x;
    float scaleY = (float)viewportSize.y / _wnd->GetFixedSize().y;

    int scaledX = (int)(x * scaleX);
    int scaledY = (int)(y * scaleY);
    int scaledWidth = (int)(width * scaleX);
    int scaledHeight = (int)(height * scaleY);

    int scissorX = viewportPos.x + scaledX;
    int scissorY = viewportPos.y + (viewportSize.y - (scaledY + scaledHeight));

    glEnable(GL_SCISSOR_TEST);
    glScissor(scissorX - viewportPos.x, scissorY, scaledWidth, scaledHeight);
}

void px::DrawingContext::Scissor(const Vec2i& pos, const Vec2i& size, WINDOW wnd)
{
    Scissor(pos.x, pos.y, size.x, size.y, wnd);
}

void px::DrawingContext::NoScissor()
{
    glDisable(GL_SCISSOR_TEST);
}
