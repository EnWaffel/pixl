#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/math/Vec2i.h"

#include <unordered_map>
#include <memory>

namespace px
{
    typedef char32_t UTFChar;
    typedef std::u32string UTFString;

    enum class FontType : uint8_t
    {
        MSDF, // Use for normal fonts (read more: https://github.com/Chlumsky/msdfgen)
        BITMAP // Use for pixel fonts
    };

    struct Glyph
    {
        unsigned int index;
        float drawAdvance;

        float advance;
        Vec2 bearing;
        Vec2 size;
    };

    class Font
    {
    public:
        PX_INTERNAL Font(void* d, void* data, uint16_t baseSize, bool antialiasing, FontType type);
        PX_INTERNAL ~Font();

        PX_API void LoadChar(UTFChar c);
        PX_API const Glyph& GetCharData(UTFChar c);
        PX_API bool HasChar(UTFChar c);
        PX_API Vec2 GetSize(UTFChar c, float textSize, float scale = 1.0f);
        PX_API Vec2 GetSize(const UTFString& str, float textSize, float scale = 1.0f);
        PX_API float GetBaseSize();
        PX_API unsigned int GetAtlas();
        PX_API FontType GetType();
    private:
        PX_INTERNAL void LoadMSDFChar(UTFChar c);
        PX_INTERNAL void LoadBitmapChar(UTFChar c);
    private:
        void* m_Data;
        void* m_Data1;
        uint16_t m_BaseSize;
        bool m_Antialiasing;
        std::unordered_map<UTFChar, Glyph> m_Glyphs;
        void* m_D;
        unsigned int m_Atlas;
        FontType m_Type;
    };

    typedef Font* FONT;

    PX_API UTFString ToUTF(CREFSTR str);
    PX_API std::string FromUTF(const UTFString& str);
}