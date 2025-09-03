#include "pixl/core/asset/Model.h"
#include "pixl/utility/Error.h"
#include "pixl/core/asset/AssetManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h>
#include <stb_image.h>

using namespace px;

static void GenOpenGLBufs(std::vector<Mesh>& meshes)
{
    for (Mesh& mesh : meshes)
    {
        unsigned int VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);  

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        mesh.data[0] = VAO;
        mesh.data[1] = VBO;
        mesh.data[2] = EBO;
    }
}

px::Model::Model()
{
}

px::Model::~Model()
{
}

static ImageData DecodeTexture(aiTexture* tex)
{
    int width, height, channels;

    stbi_uc* data = stbi_load_from_memory((stbi_uc*)tex->pcData, tex->mWidth, &width, &height, &channels, 4); // RGBA
    if (!data) return {};

    ImageData img{};
    img.format = channels == 4 ? ImageFormat::RGBA : ImageFormat::RGB;
    img.data = (uint8_t*)data;
    img.width = width;
    img.height = height;
    img.size = 1;

    return img;
}

static MeshTexture LoadEmbeddedTexture(CREFSTR modelName, const aiScene* scene, const aiString& name, CREFSTR prefix, bool antialiasing)
{
    std::string n = name.C_Str();
    int idx = std::atoi(n.substr(1).c_str());

    aiTexture* texture = scene->mTextures[idx];

    ImageData data{};

    if (texture->mHeight == 0)
    {
        data = DecodeTexture(texture);
    }
    else
    {
        data.data = (uint8_t*)texture->pcData;
        data.width = texture->mWidth;
        data.height = texture->mHeight;
        data.format = ImageFormat::RGBA;
    }

    MeshTexture tex{};
    tex.tex = AssetManager::LoadTexture("__model_texture__" + modelName + prefix, data, antialiasing);
    tex.prefix = prefix;

    if (texture->mHeight == 0) stbi_image_free(data.data);

    return tex;
}

static std::vector<MeshTexture> LoadMaterialType(CREFSTR modelName, aiMaterial* material, aiTextureType type, CREFSTR prefix, const aiScene* scene, bool antialiasing)
{
    std::vector<MeshTexture> textures;

    for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString str;
        material->GetTexture(type, i, &str);

        MeshTexture tex{};
        tex.type = (MeshTextureType)-1;

        switch (type)
        {
        case aiTextureType_DIFFUSE: tex.type = MeshTextureType::DIFFUSE; break;
        case aiTextureType_SPECULAR: tex.type = MeshTextureType::SPECULAR; break;
        case aiTextureType_NORMALS: tex.type = MeshTextureType::NORMALS; break;
        case aiTextureType_HEIGHT: tex.type = MeshTextureType::HEIGHT; break;
        default: break;
        }

        if (tex.type == (MeshTextureType)-1)
        {
            continue;
        }

        if (str.C_Str()[0] == '*')
        {
            tex = LoadEmbeddedTexture(modelName, scene, str, prefix, antialiasing);
        }
        else
        {
            tex.tex = AssetManager::LoadTexture("__model_texture__" + modelName + prefix, str.C_Str(), antialiasing);
        }

        tex.prefix = prefix;

        textures.push_back(tex);
    }

    return textures;
}

static Mesh ProcessMesh(CREFSTR modelName, aiMesh* mesh, const aiScene* scene, bool antialiasing)
{
    Mesh m{};

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
            m.indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<MeshTexture> diffuseTextures = LoadMaterialType(modelName, material, aiTextureType_DIFFUSE, "diffuse", scene, antialiasing);
        std::vector<MeshTexture> specularTextures = LoadMaterialType(modelName, material, aiTextureType_SPECULAR, "specular", scene, antialiasing);

        m.textures.insert(m.textures.end(), diffuseTextures.begin(), diffuseTextures.end());
        m.textures.insert(m.textures.end(), specularTextures.begin(), specularTextures.end());
    }

    return m;
}

static std::vector<Mesh> ProcessNode(CREFSTR modelName, aiNode* node, const aiScene* scene, MODEL model, bool antialiasing)
{
    std::vector<Mesh> meshes;

    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(modelName, mesh, scene, antialiasing));
    }
    
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        std::vector<Mesh> otherMeshes = ProcessNode(modelName, node->mChildren[i], scene, model, antialiasing);
        meshes.insert(meshes.end(), otherMeshes.begin(), otherMeshes.end());
    }

    return meshes;
}

MODEL px::Model::Load(CREFSTR modelName, std::istream& stream, bool antialiasing)
{
    stream.seekg(0, std::ios::end);
    uint32_t bufSize = stream.tellg();
    stream.seekg(0);
    
    uint8_t* buf = new uint8_t[bufSize];
    stream.read(reinterpret_cast<char*>(buf), bufSize);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFileFromMemory(buf, bufSize, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    delete[] buf;

    if (!scene)
    {
        Error::Throw(PX_ERROR, std::string("Failed to read model file (") + importer.GetErrorString() + ")");
        return nullptr;
    }

    MODEL model = new Model;

    model->m_Meshes = ProcessNode(modelName, scene->mRootNode, scene, model, antialiasing);
    GenOpenGLBufs(model->m_Meshes);

    return model;
}

Mesh px::Model::CreateMesh()
{
    return {};
}
