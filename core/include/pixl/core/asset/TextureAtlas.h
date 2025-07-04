#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/asset/Texture.h"
#include "pixl/core/math/Vec2i.h"
#include "pixl/core/math/Rect.h"

#include <unordered_map>

#define PX_ERROR_ATLAS_UNSUPPORTED_FORMAT 0x100
#define PX_ERROR_ATLAS_PARSE_ERROR 0x101
#define PX_ERROR_ATLAS_NOT_FOUND 0x102

namespace px
{
    struct SubTexture
    {
        Vec2i offset;
        Vec2i size;

        Vec2i screenOffset;
        Vec2i screenSize;
        float screenRotation;
    };

    struct AtlasMeta
    {
        std::string imagePath;
        std::unordered_map<std::string, SubTexture> subTextures;
        float imageScale = 1.0f;
    };

    class TextureAtlas
    {
    public:
        PX_INTERNAL TextureAtlas(TEXTURE texture, const std::unordered_map<std::string, SubTexture>& subTextures);
        PX_INTERNAL ~TextureAtlas();

        PX_API SubTexture Get(CREFSTR name);
        PX_API TEXTURE GetTexture();

        PX_API static AtlasMeta LoadJSONHash(CREFSTR path);
    private:
        TEXTURE m_Texture;
        std::unordered_map<std::string, SubTexture> m_SubTextures;
    };

    typedef TextureAtlas* TEXTUREATLAS;
}