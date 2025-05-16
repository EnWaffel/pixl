#include "pixl/core/graphics/Text.h"
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

    size = fnt->GetSize(text, scale);
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
		pos.y = parentSize.y / 2.0f - size.y / 2.0f;
	}
}

void px::Text::Draw(const DrawData& data)
{
    if (!fnt) return;

    data.shd1->Use();

	data.shd1->SetBool("px_flip_x", flipX);
	data.shd1->SetBool("px_flip_y", flipY);
    data.shd1->SetColor("px_color", color);
    
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
        Mat4 mat;

        float x = pos.x + offsetX + g.bearing.x;
        float y = pos.y + offsetY - g.bearing.y;

        mat.Translate(Vec2(x, y));
        mat.Scale(g.size * scale);

        data.shd1->SetMatrix4("model_matrix", mat);

        glBindTexture(GL_TEXTURE_2D, g.data);
        data.ctx->DrawQuad();

        offsetX += g.advance.x >> 6 * scale;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void px::Text::Update(float delta)
{
}

TW px::Text::TweenPos(CREFSTR id, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec2(id, to, &pos, duration, easing, delay, callback);
}

TW px::Text::TweenPos(CREFSTR id, const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec2(id, from, to, &pos, duration, easing, delay, callback);
}

TW px::Text::TweenPos(const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    CancelTween("pos");
    UpdateTweens(0.0f);
    return TweenVec2("pos", to, &pos, duration, easing, delay, callback);
}

TW px::Text::TweenPos(const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    CancelTween("pos");
    UpdateTweens(0.0f);
    return TweenVec2("pos", from, to, &pos, duration, easing, delay, callback);
}

TW px::Text::TweenSize(CREFSTR id, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec2(id, to, &size, duration, easing, delay, callback);
}

TW px::Text::TweenSize(CREFSTR id, const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec2(id, from, to, &size, duration, easing, delay, callback);
}

TW px::Text::TweenSize(const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    CancelTween("size");
    UpdateTweens(0.0f);
    return TweenVec2("size", to, &size, duration, easing, delay, callback);
}

TW px::Text::TweenSize(const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    CancelTween("size");
    UpdateTweens(0.0f);
    return TweenVec2("size", from, to, &size, duration, easing, delay, callback);
}

TW px::Text::TweenScale(CREFSTR id, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenFloat(id, to, &scale, duration, easing, delay, callback);
}

TW px::Text::TweenScale(CREFSTR id, float from, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenFloat(id, from, to, &scale, duration, easing, delay, callback);
}

TW px::Text::TweenScale(float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    CancelTween("scale");
    UpdateTweens(0.0f);
    return TweenFloat("scale", to, &scale, duration, easing, delay, callback);
}

TW px::Text::TweenScale(float from, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    CancelTween("scale");
    UpdateTweens(0.0f);
    return TweenFloat("scale", from, to, &scale, duration, easing, delay, callback);
}

TW px::Text::TweenRotation(CREFSTR id, float to, float duration, const Easing::EasingFunc &easing, float delay, const TweenCompleteCallback &callback)
{
    return TweenFloat(id, to, &rotation, duration, easing, delay, callback);
}

TW px::Text::TweenRotation(CREFSTR id, float from, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenFloat(id, from, to, &rotation, duration, easing, delay, callback);
}

TW px::Text::TweenRotation(float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    CancelTween("rotation");
    UpdateTweens(0.0f);
    return TweenFloat("rotation", to, &rotation, duration, easing, delay, callback);
}

TW px::Text::TweenRotation(float from, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    CancelTween("rotation");
    UpdateTweens(0.0f);
    return TweenFloat("rotation", from, to, &rotation, duration, easing, delay, callback);
}
