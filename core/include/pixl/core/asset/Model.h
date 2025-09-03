#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/core/math/Vec3.h"
#include "pixl/core/math/Vec2.h"
#include "pixl/core/asset/Texture.h"

#include <vector>

namespace px
{
    enum class MeshTextureType
    {
        DIFFUSE,
        SPECULAR,
        NORMALS,
        HEIGHT
    };

    struct Vertex
    {
        Vec3 pos;
        Vec3 normal;
        Vec2 texCoord;
        Vec3 tangent;
        Vec3 bitangent;
    };

    struct MeshTexture
    {
        TEXTURE tex;
        MeshTextureType type;
        std::string prefix;
    };

    struct Mesh
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<MeshTexture> textures;
        unsigned int data[3];
    };
    
    class Object3D;
    class Model;
    typedef Model* MODEL;

    class Model
    {
    public:
        PX_INTERNAL Model();
        PX_INTERNAL ~Model();

        PX_API static MODEL Load(CREFSTR modelName, std::istream& stream, bool antialiasing);
        PX_API static Mesh CreateMesh();
    protected:
        std::vector<Mesh> m_Meshes;

    friend class Object3D;
    };
}