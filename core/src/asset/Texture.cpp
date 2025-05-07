#include "pixl/core/asset/Texture.h"

#include <glad/glad.h>

using namespace px;

px::Texture::Texture(const ImageData& data) : m_Data(data)
{
}

px::Texture::~Texture()
{
    glDeleteTextures(1, &m_Data.size);
}

uint32_t px::Texture::GetWidth()
{
    return m_Data.width;
}

uint32_t px::Texture::GetHeight()
{
    return m_Data.height;
}

void px::Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, m_Data.size);
}

ImageData px::Texture::GetData()
{
    return m_Data;
}
