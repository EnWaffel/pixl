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

    struct Glyph
    {
        unsigned int data;
        Vec2i bearing;
        Vec2i size;
        Vec2i advance;
    };

    class Font
    {
    public:
        PX_INTERNAL Font(void* d, void* data, uint16_t size, bool antialiasing);
        PX_INTERNAL ~Font();

        PX_API void LoadChar(UTFChar c, uint16_t size = 0);
        PX_API const Glyph& GetCharData(UTFChar c);
        PX_API bool HasChar(UTFChar c);
        PX_API Vec2 GetSize(UTFChar c, float scale = 1.0f);
        PX_API Vec2 GetSize(const UTFString& str, float scale = 1.0f);
    private:
        void* m_Data;
        uint16_t m_Size;
        bool m_Antialiasing;
        std::unordered_map<UTFChar, Glyph> m_Glyphs;
        void* m_D;
    };

    typedef Font* FONT;

    UTFString ToUTF(CREFSTR str);
    std::string FromUTF(const UTFString& str);
};