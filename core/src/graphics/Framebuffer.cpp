#include "pixl/core/graphics/Framebuffer.h"

#include <glad/glad.h>
#include <cstring>

px::Framebuffer::Framebuffer(const Vec2i& size, bool transparent)
{
    memset(m_Handle, 0, sizeof(m_Handle));

	glGenFramebuffers(1, &m_Handle[0]);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Handle[0]);

    glGenTextures(1, &m_Handle[1]);
    glBindTexture(GL_TEXTURE_2D, m_Handle[1]);

    if (transparent)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Handle[1], 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

px::Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &m_Handle[0]);
    glDeleteTextures(1, &m_Handle[1]);
}

void px::Framebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_Handle[0]);
}

void px::Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void px::Framebuffer::BindForDrawing()
{
    glBindTexture(GL_TEXTURE_2D, m_Handle[1]);
}
