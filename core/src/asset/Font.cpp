#include "pixl/core/asset/Font.h"
#include "pixl/core/math/Vec3.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>
#include <utf8.h>
#include <vector>
#include <msdfgen.h>
#include <msdfgen-ext.h>

#define FONT_HANDLE (FontHandle*)m_Data

using namespace px;
using namespace msdfgen;

px::Font::Font(void* d, void* data, uint16_t baseSize, bool antialiasing, FontType type) : m_D(std::move(d)), m_BaseSize(baseSize), m_Antialiasing(antialiasing), m_Type(type)
{
    if (baseSize == 0) baseSize = 64;
    m_Data1 = data;
    m_Data = adoptFreetypeFont((FT_Face)data);
    
    if (type == FontType::MSDF)
    {
        glGenTextures(1, &m_Atlas);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_Atlas);
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB32F, baseSize, baseSize, 128);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }

    LoadChar('\0'); // Default char
}

px::Font::~Font()
{
    if (m_D) delete[] (uint8_t*)m_D;
    if (m_Data) msdfgen::destroyFont(FONT_HANDLE);
}

void px::Font::LoadChar(UTFChar c)
{
    if (c == U'\n') return;

    PX_DEBUG_LOG("Font::LoadChar()", "Loading glyph: '%c' (baseSize: %d)", c, m_BaseSize);

    switch (m_Type)
    {
    case FontType::MSDF: LoadMSDFChar(c); break;
    case FontType::BITMAP: LoadBitmapChar(c); break;
    default: Error::Throw(PX_ERROR_INVALID_OPERATION, "Tried to load char with invalid font type: " + std::to_string((int)m_Type)); break;
    }
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

Vec2 px::Font::GetSize(UTFChar c, float textSize, float scale)
{
    const Glyph& g = GetCharData(c);

    float width  = g.advance / 64.0f;
    float ascent = g.size.y;
    float descent = g.size.x - g.size.y;
    float height = ascent + descent;

    return Vec2(width, height);
}

Vec2 px::Font::GetSize(const UTFString& str, float textSize, float scale)
{
    float width = 0.0f;
    float maxAscent = 0;
    float maxDescent = 0;

    std::vector<float> lines;

    for (UTFChar c : str) {
        const Glyph& g = GetCharData(c);

        if (c == U'\n')
        {
            lines.push_back(width);
            width = 0.0f;
            continue;
        }

        width += m_Type == FontType::BITMAP ? g.advance : g.drawAdvance * textSize;

        float ascent = g.size.y;
        float descent = g.size.x - g.size.y;

        if (ascent > maxAscent) maxAscent = ascent;
        if (descent > maxDescent) maxDescent = descent;
    }

    lines.push_back(width);

    int i = -1;
    for (float w : lines)
    {
        if (w > width) width = w;
        i++;
    }

    return Vec2(m_Type == FontType::BITMAP ? width * scale : width, (maxAscent + maxDescent) * scale);
}

float px::Font::GetBaseSize()
{
    return m_BaseSize;
}

unsigned int px::Font::GetAtlas()
{
    return m_Atlas;
}

FontType px::Font::GetType()
{
    return m_Type;
}

void px::Font::LoadMSDFChar(UTFChar c)
{
    double advance = 0.0;
    Shape shape;
    if (!loadGlyph(shape, FONT_HANDLE, c, FONT_SCALING_EM_NORMALIZED, &advance))
    {
        Error::Throw(PX_ERROR, std::string("Failed to load glyph: '") + (char)c + "'");
        return;
    }

    shape.normalize();
    edgeColoringSimple(shape, 3);

    Bitmap<float, 3> sdf(m_BaseSize, m_BaseSize);

    FT_Set_Pixel_Sizes((FT_Face)m_Data1, 0, m_BaseSize);

    if (FT_Load_Char((FT_Face)m_Data1, c, FT_LOAD_RENDER)) {
        Error::Throw(PX_ERROR, std::string("Failed to load glyph: '") + (char)c + "'");
        return;
    }

    FT_GlyphSlot g = ((FT_Face)m_Data1)->glyph;
    float bearingY = g->bitmap_top;
    float glyphHeight = g->bitmap.rows;
    float descent = glyphHeight - bearingY;
    float padding = 0.125f;
    float offsetY = (descent / m_BaseSize) + padding;

    SDFTransformation t(Projection(m_BaseSize, Vector2(padding, offsetY)), Range(0.125));
    generateMSDF(sdf, shape, t);

    glBindTexture(GL_TEXTURE_2D_ARRAY, m_Atlas);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, m_Glyphs.size(), m_BaseSize, m_BaseSize, 1, GL_RGB, GL_FLOAT, sdf(0, 0));
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    Glyph glyph{};
    glyph.index = m_Glyphs.size();
    glyph.drawAdvance = advance;
    glyph.advance = g->advance.x;
    glyph.bearing = Vec2(g->bitmap_left, g->bitmap_top);
    glyph.size = Vec2(g->bitmap.width, g->bitmap.rows);

    m_Glyphs.insert({ c, glyph });
}

void px::Font::LoadBitmapChar(UTFChar c)
{
    FT_Face face = (FT_Face)m_Data1;

    FT_Set_Pixel_Sizes(face, 0, m_BaseSize);

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
    glyph.index = tex;
    glyph.drawAdvance = g->advance.x;
    glyph.advance = g->advance.x / 64.0f;
    glyph.bearing = Vec2(g->bitmap_left, g->bitmap_top);
    glyph.size = Vec2(g->bitmap.width, g->bitmap.rows);

    m_Glyphs.insert({ c, glyph });
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
