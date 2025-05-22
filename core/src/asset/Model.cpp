#include "pixl/core/asset/Model.h"
#include "pixl/utility/Error.h"
#include "pixl/core/asset/AssetManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace px;

px::Model::Model()
{
}

px::Model::~Model()
{
}

static std::vector<MeshTexture> LoadMaterialType(CREFSTR modelName, aiMaterial* material, aiTextureType type, CREFSTR prefix)
{
    std::vector<MeshTexture> textures;

    for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString str;
        material->GetTexture(type, i, &str);

        MeshTexture tex;
        tex.tex = AssetManager::LoadTexture("__model_texture__" + modelName + prefix, str.C_Str());
        tex.prefix = prefix;

        switch (type)
        {
        case aiTextureType_DIFFUSE: tex.type = MeshTextureType::DIFFUSE;
        case aiTextureType_SPECULAR: tex.type = MeshTextureType::SPECULAR;
        default: return textures;
        }

        textures.push_back(tex);
    }

    return textures;
}

static Mesh ProcessMesh(CREFSTR modelName, aiMesh* mesh, const aiScene* scene)
{
    Mesh m;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        aiVector3D& vertex = mesh->mVertices[i];
        aiVector3D& normal = mesh->mNormals[i];

        Vertex vert;
        vert.pos = Vec3(vertex.x, vertex.y, vertex.z);
        vert.normal = Vec3(normal.x, normal.y, normal.z);

        if (mesh->mTextureCoords[0])
        {
            vert.texCoord = Vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }

        m.vertices.push_back(vert);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace& face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            m.indices.push_back(face.mIndices[i]);
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<MeshTexture> diffuseTextures = LoadMaterialType(modelName, material, aiTextureType_DIFFUSE, "diffuse");
        std::vector<MeshTexture> specularTextures = LoadMaterialType(modelName, material, aiTextureType_SPECULAR, "specular");

        m.textures.insert(m.textures.end(), diffuseTextures.begin(), diffuseTextures.end());
        m.textures.insert(m.textures.end(), specularTextures.begin(), specularTextures.end());
    }

    return m;
}

static std::vector<Mesh> ProcessNode(CREFSTR modelName, aiNode* node, const aiScene* scene, MODEL model)
{
    std::vector<Mesh> meshes;

    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(modelName, mesh, scene));
    }
    
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(modelName, node->mChildren[i], scene, model);
    }

    return meshes;
}

MODEL px::Model::Load(CREFSTR modelName, std::istream& stream)
{
    stream.seekg(0, std::ios::end);
    uint32_t bufSize = stream.tellg();
    stream.seekg(0);
    
    uint8_t* buf = new uint8_t[bufSize];
    stream.read(reinterpret_cast<char*>(buf), bufSize);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFileFromMemory(buf, bufSize, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    delete[] buf;

    if (!scene)
    {
        Error::Throw(PX_ERROR, std::string("Failed to read model file (") + importer.GetErrorString() + ")");
        return nullptr;
    }

    MODEL model = new Model;

    model->m_Meshes = ProcessNode(modelName, scene->mRootNode, scene, model);

    return model;
}

Mesh px::Model::CreateMesh()
{
    return {};
}
