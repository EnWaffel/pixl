#include "pixl/core/asset/Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>

using namespace px;

px::Font::Font(std::unique_ptr<std::vector<uint8_t>>& d, void* data, uint16_t size, bool antialiasing) : m_D(std::move(d)), m_Data(data), m_Size(size), m_Antialiasing(antialiasing)
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

Glyph px::Font::GetCharData(UTFChar c)
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

UTFString px::ToUTF(CREFSTR str)
{
    return U"";
}

std::string px::ToStd(const UTFString& str)
{
    return "";
}
