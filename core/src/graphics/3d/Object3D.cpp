#include "pixl/core/graphics/3d/Object3D.h"

#include <glad/glad.h>

using namespace px;

px::Object3D::Object3D() : model(nullptr)
{
}

px::Object3D::Object3D(MODEL model) : model(model)
{
}

void px::Object3D::Draw(const DrawData& data)
{
    if (!model) return;

    SHADER shd = data.shaders[PX_SHD_IDX_OBJECT];
    shd->Use();
    shd->SetColor("px_color", color);

    Mat4 mat;
    mat.Translate(pos);
    mat.Scale(scale);
    mat.Rotate(rotation);

   for (Mesh& mesh : model->m_Meshes)
   {
        Mat4 finalMat = mat;
        shd->SetMatrix4("model_matrix", mat);

        unsigned int diffuse = 1, specular = 1, normals = 1, height = 1;
        for (unsigned int i = 0; i < mesh.textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            
            std::string prefix;
            MeshTextureType type = mesh.textures[i].type;

            switch (type)
            {
            case MeshTextureType::DIFFUSE: prefix = "px_diffuse" + std::to_string(diffuse++); break;
            case MeshTextureType::SPECULAR: prefix = "px_specular" + std::to_string(specular++);; break;
            case MeshTextureType::NORMALS: prefix = "px_normals" + std::to_string(normals++);; break;
            case MeshTextureType::HEIGHT: prefix = "px_height" + std::to_string(height++);; break;
            default: continue;
            }

            shd->SetInt(prefix, i);
            mesh.textures[i].tex->Bind();
        }
        
        glBindVertexArray(mesh.data[0]);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh.indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
   }
}

void px::Object3D::Update(float delta)
{
    UpdateTweens(delta);
}
