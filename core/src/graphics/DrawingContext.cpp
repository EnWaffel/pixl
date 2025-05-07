#include "pixl/core/graphics/DrawingContext.h"

#include <glad/glad.h>

using namespace px;

px::DrawingContext::DrawingContext()
{
    glGenVertexArrays(2, m_Data);
    glGenBuffers(2, m_Data1);

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

void px::DrawingContext::DrawScreenQuad()
{
	glBindVertexArray(m_Data[1]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
