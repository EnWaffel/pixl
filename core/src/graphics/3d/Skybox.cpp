#include "pixl/core/graphics/3d/Skybox.h"

#include <glad/glad.h>

using namespace px;

px::Skybox::Skybox(CUBEMAP cubeMap) : cubeMap(cubeMap)
{
}

void px::Skybox::Draw(const DrawData& data)
{
    if (!cubeMap) return;

    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    SHADER shd = data.shaders[PX_SHD_IDX_SKYBOX];
    
    Mat4 mat;
    mat.Rotate(rotation);
    
    shd->Use();
    shd->SetMatrix4("model_matrix", mat);

    glActiveTexture(GL_TEXTURE0);
    cubeMap->Bind();
    data.ctx->DrawSkybox();

    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
}

void px::Skybox::Update(float delta)
{
}
