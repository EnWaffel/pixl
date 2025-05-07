#pragma once

#include "pixl/utility/Defs.h"

namespace px
{
    enum class PX_API ImageFormat : uint8_t
    {
        RGB = 0,
        RGBA
    };

    struct PX_API ImageData
    {
        uint8_t* data;
        uint32_t size;
        uint32_t width;
        uint32_t height;
        ImageFormat format;
    };

    class Texture
    {
    public:
        PX_INTERNAL Texture(const ImageData& data);
        PX_INTERNAL ~Texture();

        PX_API uint32_t GetWidth();
        PX_API uint32_t GetHeight();

        PX_API void Bind();
        PX_API ImageData GetData();
    private:
        ImageData m_Data;
    };

    typedef Texture* TEXTURE;
}