#include "pixl/core/asset/CubeMap.h"

#include <glad/glad.h>

using namespace px;

px::CubeMap::CubeMap(unsigned int data) : m_Data(data)
{
}

px::CubeMap::~CubeMap()
{
    glDeleteTextures(1, &m_Data);
}

void px::CubeMap::Bind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_Data);
}
