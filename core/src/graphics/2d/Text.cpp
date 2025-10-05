#include "pixl/core/graphics/2d/Text.h"
#include "pixl/core/window/Window.h"

#include <glad/glad.h>

using namespace px;

extern WINDOW __pixl_rootwnd;

px::Text::Text()
{
    fnt = nullptr;
}

px::Text::Text(FONT fnt, const UTFString& text) : Text()
{
	this->fnt = fnt;
    SetText(text);
}

px::Text::Text(FONT fnt, const UTFString& text, const Vec2& pos) : Text()
{
	this->pos = pos;
    this->fnt = fnt;
    SetText(text);
}

px::Text::Text(FONT fnt, const UTFString& text, const Vec2& pos, const Vec2& scrollFactor) : Text()
{
	this->pos = pos;
	this->scrollFactor = scrollFactor;
    this->fnt = fnt;
    SetText(text);
}

px::Text::~Text()
{
}

void px::Text::SetText(const UTFString& text)
{
    if (!fnt) return;
    m_Text = text;
    
    for (UTFChar c : text)
    {
        if (!fnt->HasChar(c)) fnt->LoadChar(c);
    }

    UpdateText();
}

UTFString px::Text::GetText()
{
    return m_Text;
}

void px::Text::Center(Axis axis)
{
	if (!__pixl_rootwnd) return;
	Center(axis, __pixl_rootwnd->GetFixedSize());
}

void px::Text::Center(Axis axis, const Vec2& parentSize)
{
	Update(0.0f);

	if (axis == X || axis == XY)
	{
		pos.x = parentSize.x / 2.0f - size.x / 2.0f;
	}
	if (axis == Y || axis == XY)
	{
		pos.y = parentSize.y / 2.0f + size.y / 2.0f;
	}
}

void px::Text::UpdateText()
{
    size = fnt->GetType() == FontType::MSDF ? fnt->GetSize(m_Text, textSize, scale * textSize / fnt->GetBaseSize()) : fnt->GetSize(m_Text, textSize, scale);
}

void px::Text::Draw(const DrawData& data)
{
    if (!fnt) return;

    switch (fnt->GetType())
    {
    case FontType::MSDF:
    {
        DrawMSDF(data);
        break;
    }
    case FontType::BITMAP:
    {
        DrawBitmap(data);
        break;
    }
    }
}

void px::Text::DrawMSDF(const DrawData& data)
{
    SHADER shd = data.shaders[PX_SHD_IDX_TEXT_MSDF];
    shd->Use();

    float scale = this->scale * textSize / fnt->GetBaseSize();

	shd->SetBool("px_flip_x", flipX);
	shd->SetBool("px_flip_y", flipY);
    shd->SetColor("bgColor", bgColor);
    shd->SetColor("fgColor", fgColor);
    shd->SetColor("outlineColor", outlineColor);
    shd->SetFloat("smoothing", smoothingFactor);
    shd->SetFloat("outlineWidth", outlineWidth);
    shd->SetFloat("pxRange", 4);
    //shd->SetFloat("glowSize", glowSize);
    //shd->SetFloat("glowIntensity", glowIntensity);
    //shd->SetColor("glowColor", glowColor);
    
    float s = (fnt->GetBaseSize() * scale) / fnt->GetBaseSize();
    float screenPxRange = 4 * s;
    shd->SetFloat("screenPxRange", screenPxRange);

    glBindTexture(GL_TEXTURE_2D_ARRAY, fnt->GetAtlas());

    float offsetX = 0.0f;
    float offsetY = 0.0f;
    for (UTFChar c : m_Text)
    {
        if (c == U'\n')
        {
            offsetY += textSize + lineSpacing;
            offsetX = 0.0f;
            continue;
        }

        Glyph g = fnt->GetCharData(c);

        Vec2 _pos = pos;
        _pos.x -= data.offset.x * data.scale.x;
        _pos.y -= data.offset.y * data.scale.y;

        float x = _pos.x + offsetX;
        float y = _pos.y + offsetY + (g.size.y * scale - g.bearing.y * scale);

        float w = fnt->GetBaseSize() * scale;
        float h = fnt->GetBaseSize() * scale;

        Mat4 mat;

        mat.Translate(Vec2(x, y));
        mat.Translate(offset);
        mat.Scale(Vec2(w, h));

        shd->SetMatrix4("model_matrix", mat);
        shd->SetInt("atlas_index", g.index);
        data.ctx->DrawQuad();
        offsetX += g.drawAdvance * textSize;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void px::Text::DrawBitmap(const DrawData& data)
{
    SHADER shd = data.shaders[PX_SHD_IDX_TEXT_BITMAP];
    shd->Use();

    shd->SetBool("px_flip_x", flipX);
	shd->SetBool("px_flip_y", flipY);
    shd->SetColor("px_color", fgColor);

    float offsetX = 0.0f;
    float offsetY = 0.0f;
    for (UTFChar c : m_Text)
    {
        if (c == U'\n')
        {
            offsetY += fnt->GetCharData('\0').size.y + lineSpacing;
            offsetX = 0.0f;
            continue;
        }

        Glyph g = fnt->GetCharData(c);

        Vec2 _pos = pos;
        _pos.x -= data.offset.x * data.scale.x;
        _pos.y -= data.offset.y * data.scale.y;

        float x = _pos.x + offsetX + g.bearing.x * scale;
        float y = _pos.y + offsetY - g.bearing.y * scale;

        float w = g.size.x * scale;
        float h = g.size.y * scale;

        Mat4 mat;

        mat.Translate(Vec2(x, y));
        mat.Translate(offset);

        mat.Translate(Vec2(w / 2.0f, h / 2.0f));
        mat.Rotate(Vec3(0.0f, 0.0f, rotation));
        mat.Translate(Vec2(-(w / 2.0f), -(h / 2.0f)));

        mat.Scale(Vec2(w, h));

        shd->SetMatrix4("model_matrix", mat);

        glBindTexture(GL_TEXTURE_2D, g.index);
        data.ctx->DrawQuad();

        offsetX += g.advance * scale;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void px::Text::Update(float delta)
{
    UpdateTweens(delta);
}

TW px::Text::TweenPos(const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec2(to, &pos, duration, easing, delay, callback);
}

TW px::Text::TweenPos(const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec2(from, to, &pos, duration, easing, delay, callback);
}

TW px::Text::TweenSize(const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec2(to, &size, duration, easing, delay, callback);
}

TW px::Text::TweenSize(const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec2(from, to, &size, duration, easing, delay, callback);
}

TW px::Text::TweenScale(float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenFloat(to, &scale, duration, easing, delay, callback);
}

TW px::Text::TweenScale(float from, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenFloat(from, to, &scale, duration, easing, delay, callback);
}

TW px::Text::TweenRotation(float to, float duration, const Easing::EasingFunc &easing, float delay, const TweenCompleteCallback &callback)
{
    return TweenFloat(to, &rotation, duration, easing, delay, callback);
}

TW px::Text::TweenRotation(float from, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenFloat(from, to, &rotation, duration, easing, delay, callback);
}

// Queued tweens

TW px::Text::QueueTweenPos(const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return QueueTweenVec2(to, &pos, duration, easing, delay, callback);
}

TW px::Text::QueueTweenPos(const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return QueueTweenVec2(from, to, &pos, duration, easing, delay, callback);
}

TW px::Text::QueueTweenSize(const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return QueueTweenVec2(to, &size, duration, easing, delay, callback);
}

TW px::Text::QueueTweenSize(const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return QueueTweenVec2(from, to, &size, duration, easing, delay, callback);
}

TW px::Text::QueueTweenScale(float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return QueueTweenFloat(to, &scale, duration, easing, delay, callback);
}

TW px::Text::QueueTweenScale(float from, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return QueueTweenFloat(from, to, &scale, duration, easing, delay, callback);
}

TW px::Text::QueueTweenRotation(float to, float duration, const Easing::EasingFunc &easing, float delay, const TweenCompleteCallback &callback)
{
    return QueueTweenFloat(to, &rotation, duration, easing, delay, callback);
}

TW px::Text::QueueTweenRotation(float from, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return QueueTweenFloat(from, to, &rotation, duration, easing, delay, callback);
}
