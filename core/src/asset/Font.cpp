#include "pixl/core/asset/Font.h"
#include "pixl/core/math/Vec3.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>
#include <utf8.h>
#include <vector>

using namespace px;

px::Font::Font(void* d, void* data, uint16_t size, bool antialiasing) : m_D(std::move(d)), m_Data(data), m_Size(size), m_Antialiasing(antialiasing)
{
    if (size == 0) size = 24;
    LoadChar('\0'); // Default char
}

px::Font::~Font()
{
    for (const auto& v : m_Glyphs)
    {
        glDeleteTextures(1, &v.second.data);
    }

    if (m_D) delete[] (uint8_t*)m_D;

    if (!m_Data) return;
    FT_Done_Face((FT_Face)m_Data);
}

void px::Font::LoadChar(UTFChar c, uint16_t size)
{
    if (c == U'\n') return;

    FT_Face face = (FT_Face)m_Data;

    if (size == 0) size = m_Size;
    PX_DEBUG_LOG("Font::LoadChar()", "Loading glyph: '%c' (size: %d)", c, size);

    FT_Set_Pixel_Sizes(face, 0, size);

    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
        Error::Throw(PX_ERROR, std::string("Failed to load glyph: '") + (char)c + "'");
        return;
    }

    FT_GlyphSlot g = face->glyph;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, g->bitmap.width, g->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

    if (m_Antialiasing)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    Glyph glyph{};
    glyph.data = tex;
    glyph.advance = Vec2i(g->advance.x, g->advance.y);
    glyph.size = Vec2i(g->bitmap.width, g->bitmap.rows);
    glyph.bearing = Vec2i(g->bitmap_left, g->bitmap_top);

    m_Glyphs.insert({ c, glyph });
}

const Glyph& px::Font::GetCharData(UTFChar c)
{
    if (m_Glyphs.count(c) < 1)
    {
        return m_Glyphs.at('\0');
    }

    return m_Glyphs.at(c);
}

bool px::Font::HasChar(UTFChar c)
{
    return m_Glyphs.count(c) > 0;
}

Vec2 px::Font::GetSize(UTFChar c, float scale)
{
    const Glyph& g = GetCharData(c);

    float width  = (float)(g.advance.x) / 64.0f;
    int ascent   = g.size.y;
    int descent  = g.size.x - g.size.y;
    float height = (float)(ascent + descent);

    return Vec2(width, height);
}

Vec2 px::Font::GetSize(const UTFString& str, float scale)
{
    float width = 0.0f;
    int maxAscent = 0;
    int maxDescent = 0;

    std::vector<float> lines;

    for (UTFChar c : str) {
        const Glyph& g = GetCharData(c);

        if (c == '\n')
        {
            lines.push_back(width);
            width = 0.0f;
            continue;
        }

        width += (float)(g.advance.x) / 64.0f;

        int ascent = g.size.y;
        int descent = g.size.x - g.size.y;

        if (ascent > maxAscent) maxAscent = ascent;
        if (descent > maxDescent) maxDescent = descent;
    }

    for (float w : lines)
    {
        if (w > width) width = w;
    }

    return Vec2(width * scale, (maxAscent + maxDescent) * scale);
}

UTFString px::ToUTF(CREFSTR str)
{
    UTFString result;
    utf8::utf8to32(str.begin(), str.end(), std::back_inserter(result));
    return result;
}

std::string px::FromUTF(const UTFString& str)
{
    std::string result;
    utf8::utf32to8(str.begin(), str.end(), std::back_inserter(result));
    return result;
}
